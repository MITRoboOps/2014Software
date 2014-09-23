#ifndef GFX_INCLUDE
#define GFX_INCLUDE

extern SDL_Surface  *screen;          // Screen surface
extern SDL_Surface  *spr_logo;                        // Sprites
extern int          screen_w;         // Resolution
extern int          screen_h;
extern int          screen_bpp;
extern int          fullscreen;       // Fullscreen mode active


// Sets drawing color
void draw_color( Uint8 red, Uint8 green, Uint8 blue );

// Draws an alpha-blended pixel, used by draw_wu
void draw_pixel( short x, short y, Uint8 a );

// Draws a wu-line. Implementation, courtesy of http://www.codeproject.com/KB/GDI/antialias.aspx
void draw_wu( short X0, short Y0, short X1, short Y1 );

// Draws a popup-box
void draw_box( unsigned char x, unsigned char y, unsigned char w, unsigned char h, SDL_Surface *s );

// Draws temproray message
void draw_message( char* text );

// Load sprites
void sprites_init();

// Free sprites
void sprites_free();

// Draws out the help screen
void draw_help( int draw );


#endif
