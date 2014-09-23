#include <stdio.h>
#include <SDL/SDL.h>
#include "gfx.h"
#include "global.inc.h"
#include "cli_term.h"
#include "sdl_console.h"
#include "robocortex.h"

//#include <libswscale/swscale.h>
//#include <libavcodec/avcodec.h>
//#include "oswrap.h"




// Screen defaults
#define SCREEN_WIDTH         640 // Width
#define SCREEN_HEIGHT        480 // Height
#define SCREEN_BPP            32 // Color depth
#define SCREEN_FS              0 // Start in fullscreen

static          Uint8   draw_red, draw_green, draw_blue; // Drawing color
static    SDL_Surface  *spr_box;


          SDL_Surface  *screen;
          SDL_Surface  *spr_logo;
                  int   screen_w = SCREEN_WIDTH;         // Resolution
                  int   screen_h = SCREEN_HEIGHT;
                  int   screen_bpp = SCREEN_BPP;
                  int   fullscreen = SCREEN_FS;          // Fullscreen mode active

// Sets drawing color

void draw_color( Uint8 red, Uint8 green, Uint8 blue ) {
  draw_red = red;
  draw_green = green;
  draw_blue = blue;
}

// Draws an alpha-blended pixel, used by draw_wu
void draw_pixel( short x, short y, Uint8 a ) {
  Uint8 cr, cg, cb;
  Uint32 cv;
  if( y >= screen_h || x < 0 || x >= screen_w ) return;

  cv = ( *( Uint32* )( ( ( uint8_t* )screen->pixels ) + ( y * screen->pitch + x * 4 ) ) );

  SDL_GetRGB( cv, screen->format, &cr, &cg, &cb );
  cr = ( ( ( int )cr * a ) + ( ( int )draw_red   * ( 255 - a ) ) ) >> 8;
  cg = ( ( ( int )cg * a ) + ( ( int )draw_green * ( 255 - a ) ) ) >> 8;
  cb = ( ( ( int )cb * a ) + ( ( int )draw_blue  * ( 255 - a ) ) ) >> 8;
  cv = SDL_MapRGB( screen->format, cr, cg, cb );

  ( *( Uint32* )( ( ( uint8_t* )screen->pixels ) + ( y * screen->pitch + x * 4 ) ) ) = cv;
}

// Draws a wu-line. Implementation, courtesy of http://www.codeproject.com/KB/GDI/antialias.aspx
void draw_wu( short X0, short Y0, short X1, short Y1 ) {
   unsigned short IntensityShift, ErrorAdj, ErrorAcc;
   unsigned short ErrorAccTemp, Weighting, WeightingComplementMask;
   short DeltaX, DeltaY, Temp, XDir;
   SDL_LockSurface( screen );
   // Locked parameters
   #define BaseColor 0
   #define NumLevels 256
   #define IntensityBits 8
   // Precomputing
   if( Y0 > Y1 ) {
      Temp = Y0; Y0 = Y1; Y1 = Temp;
      Temp = X0; X0 = X1; X1 = Temp;
   }
   if( ( DeltaX = X1 - X0 ) >= 0 ) {
      XDir = 1;
   } else {
      XDir = -1;
      DeltaX = -DeltaX;
   }
   // Draw initial pixel
   draw_pixel( X0, Y0, BaseColor );
   // Draw horizontal, vertical, diagonal
   if( ( DeltaY = Y1 - Y0 ) == 0 ) {
      while( DeltaX-- != 0 ) {
         X0 += XDir;
         draw_pixel( X0, Y0, BaseColor );
      }
   } else if( DeltaX == 0 ) {
      do {
         Y0++;
         draw_pixel( X0, Y0, BaseColor );
      } while ( --DeltaY != 0 );
   } else if( DeltaX == DeltaY ) {
      do {
         X0 += XDir;
         Y0++;
         draw_pixel( X0, Y0, BaseColor );
      } while ( --DeltaY != 0 );
   } else {
     // Draw X/Y major
     ErrorAcc = 0;
     IntensityShift = 16 - IntensityBits;
     WeightingComplementMask = NumLevels - 1;
     if( DeltaY > DeltaX ) {
        ErrorAdj = ( ( unsigned long ) DeltaX << 16 ) / ( unsigned long ) DeltaY;
        while( --DeltaY ) {
           ErrorAccTemp = ErrorAcc;
           ErrorAcc += ErrorAdj;
           if( ErrorAcc <= ErrorAccTemp ) X0 += XDir;
           Y0++;
           Weighting = ErrorAcc >> IntensityShift;
           draw_pixel( X0, Y0, BaseColor + Weighting );
           draw_pixel( X0 + XDir, Y0, BaseColor + ( Weighting ^ WeightingComplementMask ) );
        }
     } else {
       ErrorAdj = ( ( unsigned long) DeltaY << 16) / (unsigned long) DeltaX;
       while( --DeltaX ) {
          ErrorAccTemp = ErrorAcc;
          ErrorAcc += ErrorAdj;
          if ( ErrorAcc <= ErrorAccTemp ) Y0++;
          X0 += XDir;
          Weighting = ErrorAcc >> IntensityShift;
          draw_pixel( X0, Y0, BaseColor + Weighting );
          draw_pixel( X0, Y0 + 1, BaseColor + ( Weighting ^ WeightingComplementMask ) );
       }
     }
     draw_pixel( X1, Y1, BaseColor );
   }
   SDL_UnlockSurface( screen );
}

// Draws a popup-box
void draw_box( unsigned char x, unsigned char y, unsigned char w, unsigned char h, SDL_Surface *s ) {
  SDL_Rect src, dst;
  unsigned char xx, yy;
  SDL_BlitSurface( spr_box, rect( &src, 0, 0, 32, 32 ), s, rect( &dst, x << 4, y << 4, 32, 32 ) );
  SDL_BlitSurface( spr_box, rect( &src, 48, 0, 32, 32 ), s, rect( &dst, ( x + w - 2 ) << 4, y << 4, 32, 32 ) );
  SDL_BlitSurface( spr_box, rect( &src, 0, 48, 32, 32 ), s, rect( &dst, x << 4, ( y + h - 2 ) << 4, 32, 32 ) );
  SDL_BlitSurface( spr_box, rect( &src, 48, 48, 32, 32 ), s, rect( &dst, ( x + w - 2 ) << 4, ( y + h - 2 ) << 4, 32, 32 ) );
  for( xx = x + 2; xx < x + w - 2; xx++ ) {
    SDL_BlitSurface( spr_box, rect( &src, 32, 0, 16, 32 ), s, rect( &dst, xx << 4, y << 4, 16, 16 ) );
    SDL_BlitSurface( spr_box, rect( &src, 32, 48, 16, 32 ), s, rect( &dst, xx << 4, ( y + h - 2 ) << 4, 16, 16 ) );
  }
  for( yy = y + 2; yy < y + h - 2; yy++ ) {
    SDL_BlitSurface( spr_box, rect( &src, 0, 32, 32, 16 ), s, rect( &dst, x << 4, yy << 4, 16, 16 ) );
    SDL_BlitSurface( spr_box, rect( &src, 48, 32, 32, 16 ), s, rect( &dst, ( x + w - 2 ) << 4, yy << 4, 16, 16 ) );
    for( xx = x + 2; xx < x + w - 2; xx++ ) {
      SDL_BlitSurface( spr_box, rect( &src, 32, 32, 16, 16 ), s, rect( &dst, xx << 4, yy << 4 , 16, 16 ) );
    }
  }
}

// Draws temproray message
void draw_message( char* text ) {
  term_write( 1, term_h - 3, text, FONT_RED );
  message_timeout = 125;
}

// Load sprites
void sprites_init() {
  spr_logo = SDL_LoadBMP( "logo.bmp" );
  if( !spr_logo ) printf( "RoboCortex [error]: Unable to load logo.bmp\n" );
  spr_logo = SDL_DisplayFormat( spr_logo );
  spr_box = SDL_LoadBMP( "box.bmp" );
  if( !spr_box ) printf( "RoboCortex [error]: Unable to load box.bmp\n" );
  /* // Use bitmap "reserved" as alpha:
  spr_box->format->Amask = 0xFF000000;
  spr_box->format->Ashift = 24;
  spr_box->flags |= SDL_SRCALPHA;
  */
  // Use magenta color keying:
  spr_box = SDL_DisplayFormat( spr_box );
  SDL_SetColorKey( spr_box, SDL_SRCCOLORKEY, SDL_MapRGB( screen->format, 0xFF, 0x00, 0xFF ) );
}

// Free sprites
void sprites_free() {
    SDL_FreeSurface( spr_logo );
}

// Draws out the help screen
void draw_help( int draw ) {
  unsigned char n;
  unsigned char y = ( term_h - help_count ) >> 1;
  for( n = 0; n < help_count; n++ ) {
    if( draw ) {
     term_write( ( term_w - 32 ) >> 1, y + n, help[ n ], FONT_GREEN );
    } else {
      term_white( ( term_w - 32 ) >> 1, y + n, 32 );
    }
  }
}

/* == CURSOR HELPERS ============================================================================ */

// Grab the cursor (locks cursor to our app)
void cursor_grab( int b_grab ) {
  SDL_GrabMode ret;
  ret = SDL_WM_GrabInput( b_grab ? SDL_GRAB_ON : SDL_GRAB_OFF );
  cursor_grabbed = ( ret & SDL_GRAB_ON ) != 0;
  SDL_ShowCursor( cursor_grabbed ? SDL_DISABLE : SDL_ENABLE );
}

// Poll cursor with warping support and grabbing detection
// Adds any movement to incremental variables ix and iy
void cursor_poll( int32_t *ix, int32_t *iy ) {
  static int ready = 0;
  static int lx = 0, ly = 0;
  int cx = 0, cy = 0;
  int warp = 0;

  if( cursor_hook != NULL || !cursor_grabbed ) ready = 0;
  SDL_GetMouseState( &cx, &cy );
  if( ready ) {
    *ix += cx - lx;
    *iy += cy - ly;
    if( cx > ( ( screen_w >> 1 ) + ( screen_w >> 2 ) )
     || cx < ( ( screen_w >> 1 ) - ( screen_w >> 2 ) ) ) {
      cx = screen_w >> 1;
      warp = 1;
    }
    if( cy > ( ( screen_h >> 1 ) + ( screen_h >> 2 ) )
     || cy < ( ( screen_h >> 1 ) - ( screen_h >> 2 ) ) ) {
      cy = screen_h >> 1;
      warp = 1;
    }
    if( warp ) SDL_WarpMouse( cx, cy );
  }
  if( cursor_grabbed ) ready = 1;
  lx = cx; ly = cy;
}
