#include <stdio.h>
#include <SDL/SDL.h>
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#include "global.inc.h"
#include "oswrap.h"
#include "robocortex.h"
#include "cli_term.h"
#include "plugins/cli.h"
#include "sdl_console.h"
#include "gfx.h"


// Plugins
#define MAX_PLUGINS           16
extern pluginclient_t *kiwiray_open( pluginhost_t* );
extern pluginclient_t *monitor_open( pluginhost_t* );
extern pluginclient_t *ipv4udp_open( pluginhost_t* );

// Protocol
#define MAX_RETRY              5 // Maximum number of retransmissions of lost packets

// Configuration
#define CLIENT_RPS            50 // Client refreshes per second

// Timeouts (in refreshes, see CLIENT_RPS)
#define TIMEOUT_TRUST         16 // Before retransmitting trusted packets
#define TIMEOUT_STREAM       125 // Before considering connection lost


// Client state
enum state_e {
  STATE_CONNECTING,
  STATE_QUEUED,
  STATE_ERROR,
  STATE_FULL,
  STATE_LOST,
  STATE_VERSION,
  STATE_STREAMING
};

// Keyboard mapping
enum km_e {
  KM_LEFT,
  KM_RIGHT,
  KM_UP,
  KM_DOWN,
  KM_SIZE
};

// Exit code list
enum exitcode_e {
  EXIT_OK,
  EXIT_CONFIG,
  EXIT_DECODER,
  EXIT_SURFACE,
  EXIT_COMMS
};



// Locals
static    SDL_Joystick *joy = NULL;                      // Joystick
static    disp_data_t   disp_data;                       // Data from latest DISP packet


static   linked_buf_t  *p_buffer_last;                   // Decoding buffer
static   linked_buf_t  *p_buffer_first;
static  volatile  int   state = STATE_CONNECTING;        // Client state
static  volatile  int   retry = 0;                       // Used for retransmissions and timeouts
static            int   queue_time;                      // Time left before FUN
static   linked_buf_t  *trust_first = NULL;              // Non-lossy packet buffer
static   linked_buf_t  *trust_last = NULL;
static  unsigned char   trust_srv = 0xFF;                // Non-lossy transmission counters
static  unsigned char   trust_cli = 0x00;
static      SDL_mutex  *trust_mx;                        // Non-lossy buffer access mutex
static            int   trust_timeout = 0;               // Non-lossy retransmission timeout
                // Cursor is grabbed
static         SDLKey   keymap[ KM_SIZE ];               // Keyboard remapping

static    ctrl_data_t   ctrl;                            // Part of CTRL packet
static            int   help_shown;                      // Help is displayed
static           void   ( *comm_send )( char*, int );    // Communications handler
static           char  voice[ 256 ] = "default";


// Plugins
static   pluginhost_t  host;
static pluginclient_t *plug;
static pluginclient_t *plugs[ MAX_PLUGINS ];
static            int  plugs_count;

static pluginclient_t *keyboard_hook;
static pluginclient_t *keyboard_binds[ SDLK_LAST ];

// Configuration
static           char config_default[] = "cli.rc"; // Default configuration file

/* == TRUSTED COMMUNICATIONS ==================================================================== */

// Queues a packet for trusted (non-lossy) transmission
static void trust_queue( uint32_t ident, void* data, unsigned char size ) {
  linked_buf_t *p_trust;
  p_trust = malloc( sizeof( linked_buf_t ) );
  if( p_trust ) {
    // Create packet
    memcpy( p_trust->data, &ident, 4 );
    p_trust->data[ 4 ] = size;
    memcpy( p_trust->data + 5, data, size );
    p_trust->size = size + 5;
    p_trust->next = NULL;
    // Insert into linked list
    SDL_mutexP( trust_mx );
    if( trust_first ) {
      trust_last->next = p_trust;
    } else {
      trust_first = p_trust;
    }
    trust_last = p_trust;
    SDL_mutexV( trust_mx );
  }
}

// Handles trusted data packets
static void trust_handler( char* data, int size ) {
  int len, pid;
  uint32_t ident;
  if( size == 0 ) return;
  trust_srv++;
  // Pass data to plugins
  while( size > 5 ) {
    ident = *( uint32_t* )data; data += 4;
    len = *data++;
    size -= 5;
    if( size >= len ) {
      // plugin->recv
      for( pid = 0; pid < MAX_PLUGINS && ( plug = plugs[ pid ] ) != NULL; pid++ ) {
        if( plug->ident == ident ) {
          if( plug->recv ) plug->recv( data, len );
        }
      }
    }
    size -= len;
  }
}
/* == JOYSTICK HELPERS ========================================================================== */
static void joystick_init() {
  joy = SDL_JoystickOpen( 0 );
}

static int joystick_axis( int axis, int dz, int div ) {
  axis = SDL_JoystickGetAxis( joy, axis );
  if( axis < dz && axis > -dz ) {
    axis = 0;
  } else {
    axis -= ( axis > 0 ? dz : -dz );
  }
  return( axis / div );
}

static void joystick_poll( int32_t *mx, int32_t *my, int32_t *ix, int32_t *iy ) {
  //SDL_JoystickUpdate();
  *mx  = joystick_axis( 0, 500,  256 );
  *my  = joystick_axis( 1, 500,  256 );
  *ix += joystick_axis( 2, 500, 1000 );
  *iy += joystick_axis( 3, 500, 1000 );
}

static void joystick_close() {
  if( joy ) SDL_JoystickClose( joy );
  joy = NULL;
}

/* == COMMUNICATIONS ============================================================================ */

// Processes a data packet
static void comm_recv( char *buffer, int size ) {
  linked_buf_t *p_buffer_temp;
  if( size >= 4 ) {
    memcpy( p_buffer_last->data, buffer, size );

    // H264
    if( memcmp( p_buffer_last->data, pkt_h264, 4 ) == 0 ) {
      // h264 packet
      state = STATE_STREAMING;
      retry = 0;

      // Push decoder buffer to queue
      p_buffer_last->next = malloc( sizeof( linked_buf_t ) );
      linked_buf_t *p_buf = p_buffer_last;
      p_buffer_last = p_buffer_last->next;
      p_buffer_last->size = 0;
      p_buf->size = size;

    // DATA
    } else if( memcmp( p_buffer_last->data, pkt_data, 4 ) == 0 ) {
      if( size >= 4 + sizeof( disp_data_t ) ) {
        memcpy( &disp_data, p_buffer_last->data + 4, sizeof( disp_data_t ) );

        // Check if outgoing trusted data recieved, free trusted buffers
        SDL_mutexP( trust_mx );
        if( trust_first ) {
          if( disp_data.trust_cli == trust_cli ) {
            linked_buf_t* p_trust = trust_first;
            trust_first = trust_first->next;
            trust_cli++;
            trust_timeout = 0;
            free( p_trust );
          }
        }
        SDL_mutexV( trust_mx );

        // Handle incoming trusted data
        if( ( ( trust_srv + 1 ) & 0xFF ) == disp_data.trust_srv ) {
          trust_handler( p_buffer_last->data + 4 + sizeof( disp_data_t ), size - 4 - sizeof( disp_data_t ) );
        }
      }

    // HELO
    } else if( memcmp( p_buffer_last->data, pkt_helo, 4 ) == 0 ) {
      if( state == STATE_CONNECTING ) {

        // Go to queued only if version is correct
        if( p_buffer_last->data[ 4 ] != CORTEX_VERSION ) {
          state = STATE_VERSION;
        } else {
          state = STATE_QUEUED;
          retry = 0;
          queue_time = *( int* )&p_buffer_last->data[ 5 ];
        }
      }

    // TIME
    } else if( memcmp( p_buffer_last->data, pkt_time, 4 ) == 0 ) {

      // Update queue time
      if( state == STATE_QUEUED ) {
        retry = 0;
        queue_time = *( int* )&p_buffer_last->data[ 4 ];
      }

    // LOST
    } else if( memcmp( p_buffer_last->data, pkt_lost, 4 ) == 0 ) {

      // Connection was lost (server don't know who we are)
      state = STATE_LOST;

    // FULL
    } else if( memcmp( p_buffer_last->data, pkt_full, 4 ) == 0 ) {

      // Connection could not be established (server queue is full)
      state = STATE_FULL;
    }
  }
}

/* == CONFIGURATION ============================================================================= */

static int config_set( char *value, char *token ) {
  if( token != NULL ) {
    if( strcmp( token, "width" ) == 0 ) {
      screen_w = atoi( value );
    } else if( strcmp( token, "height" ) == 0 ) {
      screen_h = atoi( value );
    } else if( strcmp( token, "bpp" ) == 0 ) {
      screen_bpp = atoi( value );
    } else if( strcmp( token, "fullscreen" ) == 0 ) {
      fullscreen = atoi( value );
    } else if( strcmp( token, "plugin" ) == 0 ) {
      return( 1 );
    } else printf( "Config [warning]: unknown entry %s\n", token );
  }
  return( 0 );
}

/* == PLUGIN SYSTEM ============================================================================= */

static int plug_keybind( int key ) {
  // TODO: block reserved keys
  if( keyboard_binds[ key ] == NULL ) {
    keyboard_binds[ key ] = plug;
    return( 1 );
  }
  return( 0 );
}

static void plug_keyfree( int key ) {
  if( keyboard_binds[ key ] == plug ) keyboard_binds[ key ] = NULL;
}

static int plug_keyhook() {
  if( keyboard_hook == NULL ) {
    ctrl.ctrl.kb = 0;
    keyboard_hook = plug;
    return( 1 );
  }
  return( 0 );
}

static void plug_keyrelease() {
  if( keyboard_hook == plug ) keyboard_hook = NULL;
}

static int plug_csrhook( int show ) {
  if( cursor_hook == NULL ) {
    cursor_hook = plug;
    SDL_ShowCursor( show ? SDL_ENABLE : SDL_DISABLE );
    return( 1 );
  }
  return( 0 );
}

static void plug_csrrelease() {
  if( cursor_hook == plug ) {
    cursor_hook = NULL;
    SDL_ShowCursor( cursor_grabbed ? SDL_DISABLE : SDL_ENABLE );
  }
}

static void plug_csrmove( int x, int y ) {
  if( cursor_hook == plug ) SDL_WarpMouse( x, y );
}

static void plug_send( void *data, unsigned char size ) {
  trust_queue( plug->ident, data, size );
}

static void plug_help( char *text ) {
  if( strlen( text ) <= 32 && help_count < 16 ) strcpy( help[ help_count++ ], text );
}

static void plug_wu( int x0, int y0, int x1, int y1, uint32_t color ) {
  draw_color( color >> 16, color >> 8, color );
  draw_wu( x0, y0, x1, y1 );
}

static int plug_cfg( char* dst, char* req_token ) {
  return( config_plugin( plug->ident, dst, req_token ) );
}

static int plug_thread( void *pThread ) {
  return( ( ( int( * )() )pThread )() );
}

static void* plug_thrstart( int( *pThread )() ) {
  return( SDL_CreateThread( plug_thread, ( void* )pThread ) );
}

static void plug_thrstop( void* pHandle ) {
  SDL_KillThread( pHandle );
}

static void plug_thrdelay( int delay ) {
  SDL_Delay( delay );
}

static void speak_voice( char* v ) {
  strcpy( voice, v );
}

static void speak_text( char* text ) {
  //Do nothing
}

static void load_plugins() {
  int pid;
  host.thread_start     = plug_thrstart;
  host.thread_stop      = plug_thrstop;
  host.thread_delay     = plug_thrdelay;
  host.cfg_read         = plug_cfg;
  host.key_bind         = plug_keybind;
  host.key_free         = plug_keyfree;
  host.keyboard_hook    = plug_keyhook;
  host.keyboard_release = plug_keyrelease;
  host.cursor_hook      = plug_csrhook;
  host.cursor_release   = plug_csrrelease;
  host.cursor_move      = plug_csrmove;
  host.text_cins        = term_cins;
  host.text_crem        = term_crem;
  host.text_write       = term_write;
  host.text_clear       = term_white;
  host.text_valid       = term_knows;
  host.server_send      = plug_send;
  host.help_add         = plug_help;
  host.speak_voice      = speak_voice;
  host.speak_text       = speak_text;
  host.draw_wuline      = plug_wu;
  host.draw_box         = draw_box;
  host.draw_message     = draw_message;
  host.text_cols        = term_w;
  host.text_rows        = term_h;
  host.comm_recv        = comm_recv;

  printf( "RoboCortex [info]: Loading plugins...\n" );
  // Load plugins
  plugs[ plugs_count++ ] = kiwiray_open( &host );
  plugs[ plugs_count++ ] = monitor_open( &host );
  plugs[ plugs_count++ ] = ipv4udp_open( &host );
  printf( "RoboCortex [info]: Initializing plugins...\n" );
  // plugin->init
  for( pid = 0; pid < MAX_PLUGINS && ( plug = plugs[ pid ] ) != NULL; pid++ ) {
    if( config_plugin( plug->ident, NULL, NULL ) ) {
      if( plug->init ) plug->init();
    } else {
      memset( plug, 0, sizeof( pluginclient_t ) );
    }
    if( plug->comm_send ) {
      if( comm_send ) printf( "RoboCortex [error]: Multiple communications plugins loaded\n" );
      comm_send = plug->comm_send;
    }
  }
  printf( "RoboCortex [info]: Plugins loaded and initialized\n" );
}

void speak_viseme( uint8_t viseme ) {
}

static void unload_plugins() {
  int pid;
  // plugin->close
  for( pid = 0; pid < MAX_PLUGINS && ( plug = plugs[ pid ] ) != NULL; pid++ )
    if( plug->close ) plug->close();
}

/* == MAIN THREAD =============================================================================== */

int main( int argc, char *argv[] ) {
  int                pid;                        // Plugin iteration
  int                temp;                       // Various uses
  int                statec = 0;                 // State counter, used for retransmissions
  int                laststate = -1;             // Used to detect state changes
  char               ascii;                      // Used for unicode text input translation
  char               p_ctrl[ 8192 ];             // CTRL packet buffer
  int                i_ctrl;                     // Tracks size of p_ctrl
  AVCodecContext    *pCodecCtx;                  // FFMPEG codec context
  AVCodec           *pCodec;                     // Pointer to FFMPEG codec (H264)
  AVFrame           *pFrame;                     // Used in the decoding process
  struct SwsContext *convertCtx;                 // Used in the scaling/conversion process
  AVPacket           avpkt;                      // Used in the decoding process
  SDL_Rect           r;                          // Used for various graphics operations
  SDL_Surface       *live = NULL;                // Live decoded video surface
  char              *p_vis;                      // Pointer to speech visualization data
  Uint32             rmask, gmask, bmask, amask; // Masking (endianness)
  SDL_Event          event;                      // Events
  int                quit = 0;                   // Time to quit?
  Uint32             time_target;                // Timing target
  Sint32             time_diff;                  // Timing differential
  FILE              *cf;                         // Configuration file
  int                do_decode;                  // Frame available for decoding

  printf( "-- RoboOps Pilot Client --\n" );
  printf( "  -- MIT RoboTeam 2014 --\n\n" );
  printf( "Based on RoboCortex by stg, phrst and jonasb of Forskningsavdelningen\n" );
  printf( "Modified by: Reo Baird\n" );
  printf( "Revision: 1A (April 20, '14)\n" );

  // Read configuration file
  config_rc = ( argc > 1 ? argv[ 1 ] : config_default );
  config_parse( config_set );

  // Validate Configuration
  if( screen_w & ~3 != screen_w || screen_h & ~3 != screen_h ) {
    printf( "Config [error]: Width and height must be a multiple of 16\n" );
    exit( EXIT_CONFIG );
  }
  if( screen_w > 4096 || screen_h > 4096 ) {
    printf( "Config [error]: Width or height must not exceed 4096\n" );
    exit( EXIT_CONFIG );
  }
  if( screen_w < 640 || screen_h < 480 ) {
    printf( "Config [error]: Minimum resolution is 640x480\n" );
    exit( EXIT_CONFIG );
  }


  // Initialize decoder
  avcodec_init();
  avcodec_register_all();
  pCodecCtx = avcodec_alloc_context();
  pCodec = avcodec_find_decoder( CODEC_ID_H264 );
  av_init_packet( &avpkt );
  if( !pCodec ) {
    printf( "ERROR: Unable to initialize decoder\n" );
    exit( EXIT_DECODER );
  }
  avcodec_open( pCodecCtx, pCodec );

  // Allocate decoder frame
  pFrame = avcodec_alloc_frame();

  SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK );
  SDL_WM_SetCaption( "Rover Client", "RoverClient" );

  // Initlalize joystick
  joystick_init();

  screen = SDL_SetVideoMode( screen_w, screen_h, screen_bpp, ( fullscreen ? SDL_FULLSCREEN : 0 ) );

  // TODO: need check if fullscreen was possible?
  cursor_grab( fullscreen );

  SDL_EnableUNICODE( SDL_ENABLE );
  SDL_EnableKeyRepeat( 400, 50 );

  sprites_init();
  term_init( screen );

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

  SDL_Surface* frame = SDL_CreateRGBSurface( SDL_SWSURFACE, screen_w, screen_h, 24, 0, 0, 0, 0 );

  if( !frame ) {
    printf( "ERROR: Unable to allcate SDL surface\n" );
    exit( EXIT_SURFACE );
  }

  p_buffer_last = malloc( sizeof( linked_buf_t ) );;
  p_buffer_first = p_buffer_last;
  p_buffer_last->size = 0;

  trust_mx = SDL_CreateMutex();

  // Load plugins
  load_plugins();
  atexit( unload_plugins );
  if( comm_send == NULL ) {
    printf( "ERROR: No communications plugin loaded\n" );
    exit( EXIT_COMMS );
  }

  time_target = SDL_GetTicks();
  //Begin Main Loop
  while( !quit ) {
    cursor_poll( &ctrl.ctrl.mx, &ctrl.ctrl.my );
    joystick_poll( &ctrl.ctrl.dx, &ctrl.ctrl.dy, &ctrl.ctrl.mx, &ctrl.ctrl.my );
    do_decode = 0;

    if( state != laststate ) {
      if( state == STATE_STREAMING )
        ctrl.ctrl.kb = 0;
      if( keyboard_hook )
        if( ( plug = keyboard_hook )->lost )
            plug->lost();
      if( cursor_hook )
        if( ( plug = cursor_hook )->lost && plug != keyboard_hook )
            plug->lost();

      keyboard_hook = NULL;
      cursor_hook = NULL;
      term_crem();
      laststate = state;

      term_clear();
      draw_help( help_shown );
      // Initialize view
      if( state != STATE_STREAMING ) {
        switch( state ) {
          case STATE_CONNECTING:
            term_write( ( term_w - 22 ) >> 1, ( term_h >> 1 ) + 9, text_contact, FONT_GREEN );
            term_write( ( term_w - 22 ) >> 1, ( term_h >> 1 ) + 10, text_blank, FONT_GREEN );
            break;
          case STATE_QUEUED:
            term_write( ( term_w - 22 ) >> 1, ( term_h >> 1 ) + 9, text_queued, FONT_GREEN );
            break;
          case STATE_ERROR:
            term_write( ( term_w - 22 ) >> 1, ( term_h >> 1 ) + 9, text_error, FONT_RED );
            term_write( ( term_w - 22 ) >> 1, ( term_h >> 1 ) + 10, text_blank, FONT_GREEN );
            break;
          case STATE_FULL:
            term_write( ( term_w - 22 ) >> 1, ( term_h >> 1 ) + 9, text_full, FONT_RED );
            term_write( ( term_w - 22 ) >> 1, ( term_h >> 1 ) + 10, text_blank, FONT_GREEN );
            break;
          case STATE_LOST:
            term_write( ( term_w - 22 ) >> 1, ( term_h >> 1 ) + 9, text_lost, FONT_RED );
            term_write( ( term_w - 22 ) >> 1, ( term_h >> 1 ) + 10, text_blank, FONT_GREEN );
            break;
          case STATE_VERSION:
            term_write( ( term_w - 22 ) >> 1, ( term_h >> 1 ) + 9, text_version, FONT_RED );
            term_write( ( term_w - 22 ) >> 1, ( term_h >> 1 ) + 10, text_blank, FONT_GREEN );
            break;
        }
        term_write( ( term_w - 22 ) >> 1, ( term_h >> 1 ) + 11, text_quit, FONT_GREEN );
      }
    }

    if( state == STATE_STREAMING ) {

      // Connected & streaming, buld CTRL packet
      memcpy( p_ctrl, pkt_ctrl, 4 );
      i_ctrl = 4;
      ctrl.trust_cli = trust_cli;
      ctrl.trust_srv = trust_srv;
      memcpy( p_ctrl + 4, &ctrl, sizeof( ctrl_data_t ) );
      i_ctrl += sizeof( ctrl_data_t );

      // Append trusted data if any
      SDL_mutexP( trust_mx );
      if( trust_timeout == 0 ) {
        if( trust_first ) {
          memcpy( p_ctrl + i_ctrl, trust_first->data, trust_first->size );
          i_ctrl += trust_first->size;
          trust_timeout = TIMEOUT_TRUST;
        }
      } else {
        trust_timeout--;
      }
      SDL_mutexV( trust_mx );

      // Send CTRL packet
      comm_send( p_ctrl, i_ctrl );
      if( ++retry == TIMEOUT_STREAM ) state = STATE_LOST;

      if( p_buffer_first->size ) {
        do_decode = 1;
        // Decode frame
        avpkt.data = ( unsigned char* )p_buffer_first->data;
        avpkt.size = p_buffer_first->size;
        avpkt.flags = AV_PKT_FLAG_KEY;
        if( avcodec_decode_video2( pCodecCtx, pFrame, &temp, &avpkt ) < 0 ) {
          printf( "NOTICE: Decoding error (packet loss)\n" );
        } else {
          SDL_LockSurface( frame );

          const uint8_t * data[1] = { frame->pixels };
          int linesize[1] = { frame->pitch };

          // Create scaling & color-space conversion context
          convertCtx = sws_getContext( pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
            frame->w, frame->h, PIX_FMT_RGB24, SWS_AREA, NULL, NULL, NULL);

          // Scale and convert the frame
          sws_scale( convertCtx, (const uint8_t**) pFrame->data, pFrame->linesize, 0,
            pCodecCtx->height, (uint8_t * const*) data, linesize );

          // Cleanup
          sws_freeContext( convertCtx );

          SDL_UnlockSurface( frame );

          // Convert to display format for fast blitting
          if( live ) SDL_FreeSurface( live );
          live = SDL_DisplayFormat( frame );

          // Update control timer
          term_write( 1, 1, text_controls, FONT_GREEN );
          if( disp_data.timer == 0 ) {
            term_white( 1, 2, strlen( text_timeout ) );
          } else {
            temp = disp_data.timer / 25;
            text_timeout[ 15 ] = '0' + ( ( temp % 60 ) % 10 );
            text_timeout[ 14 ] = '0' + ( ( temp % 60 ) / 10 );
            temp /= 60;
            text_timeout[ 12 ] = '0' + ( ( temp % 60 ) % 10 );
            text_timeout[ 11 ] = '0' + ( ( temp % 60 ) / 10 );
            term_write( 1, 2, text_timeout, FONT_GREEN );
          }
        }

        // Pop decoding buffer from queue
        linked_buf_t *p_buf = p_buffer_first;
        p_buffer_first = p_buffer_first->next;
        free( p_buf );

      }

      if( live ) {
        // Draw video
        SDL_BlitSurface( live, NULL, screen, NULL );
        } else {
        // Clear screen
        SDL_FillRect( screen, rect( &r, 0, 0, screen->w, screen->h ), 0 );
        }

    } else {

      // Clear screen
      SDL_FillRect( screen, rect( &r, 0, 0, screen->w, screen->h ), 0 );
      // Draw logo
      SDL_BlitSurface( spr_logo, NULL, screen, rect( &r, ( screen_w - spr_logo->w ) >> 1, ( screen_h >> 1 ) - spr_logo->h + 80, 0, 0 ) );

      switch( state ) {
        case STATE_CONNECTING:
          if( statec == 0 ) {
            if( ++retry == MAX_RETRY ) {
              state = STATE_ERROR;
            } else {
              comm_send( pkt_helo, 4 );
            }
          }
          break;
        case STATE_QUEUED:

          // Draw queue time
          temp = queue_time / 25;

          text_time[ 14 ] = '0' + ( ( temp % 60 ) % 10 );
          text_time[ 13 ] = '0' + ( ( temp % 60 ) / 10 );
          temp /= 60;
          text_time[ 11 ] = '0' + ( ( temp % 60 ) % 10 );
          text_time[ 10 ] = '0' + ( ( temp % 60 ) / 10 );
          temp /= 60;
          text_time[  8 ] = '0' + ( temp % 10 );
          text_time[  7 ] = '0' + ( temp / 10 );
          term_write( ( term_w - 22 ) >> 1, ( term_h >> 1 ) + 10, text_time, FONT_GREEN );
          if( statec == 0 ) {
            if( ++retry == MAX_RETRY ) {
              state = STATE_ERROR;
            } else {
              comm_send( pkt_time, 4 );
            }
          }Log::Write("Packed Received", Log::VERBOSE_DEBUG);
          break;
      }
      if( statec ) statec--; else statec = 100;

    }

    // Allow plugins to draw
    for( pid = 0; pid < MAX_PLUGINS && ( plug = plugs[ pid ] ) != NULL; pid++ )
      if( plug->draw ) plug->draw( screen );

    // Draw help overlay
    if( help_shown ) draw_box( ( term_w - 34 ) >> 1, ( term_h - 2 - help_count ) >> 1, 34, help_count + 2, screen );
SDLK_BACKQUOTE
    // Clear messages
    if( message_timeout ) {
      if( --message_timeout == 0 ) term_white( 1, term_h - 3, 38 );
    }

    // Draw terminal overlay
    term_draw();

    // Refresh screen
    SDL_UpdateRect( screen, 0, 0, 0, 0 );

    while( SDL_PollEvent( &event ) && !quit ) {
      switch( event.type ) {
        case SDL_QUIT:
          quit = 1; // Set time to quit
          break;

        case SDL_ACTIVEEVENT:
          if( event.active.state & SDL_APPINPUTFOCUS ) {
            if( event.active.gain == 1 ) {
              cursor_grab( fullscreen ); // Window gained focus
            } else {
              cursor_grab( 0 ); // Window lost focus
            }
          }
          break;

        case SDL_MOUSEMOTION:
          if( state == STATE_STREAMING && cursor_hook != NULL ) {
            plug = cursor_hook; // Notify plugin
            if( plug->cursor ) plug->cursor( E_MOVE, event.motion.x, event.motion.y );
          }
          break;

        case SDL_MOUSEBUTTONDOWN:
          if( cursor_hook == NULL ) {
            // Toggle cursor grabbing
            if( event.button.button == SDL_BUTTON_LEFT && !fullscreen ) {
              cursor_grab( !cursor_grabbed );
            }
          } else if( state == STATE_STREAMING ) {
            plug = cursor_hook; // Notify plugin
            if( plug->cursor ) plug->cursor( E_BUTTONDOWN, event.motion.x, event.motion.y );
          }
          break;

        case SDL_MOUSEBUTTONUP:
          if( state == STATE_STREAMING && cursor_hook != NULL ) {
            plug = cursor_hook; // Notify plugin
            if( plug->cursor ) plug->cursor( E_BUTTONUP, event.motion.x, event.motion.y );
          }
          break;

        case SDL_KEYDOWN:
          if( keyboard_hook == NULL ) {
            switch( event.key.keysym.sym ) {
              case SDLK_ESCAPE: // Quit
                quit = 1;
                break;

              case SDLK_f: // Toggle fullscreen
                fullscreen = !fullscreen;
                screen = SDL_SetVideoMode( screen_w, screen_h, screen_bpp, ( fullscreen ? SDL_FULLSCREEN : 0 ) );
                // TODO: need check if fullscreen was possible?
                cursor_grab( fullscreen );
                laststate = -1;
                break;

              case SDLK_BACKQUOTE: // Switch keyboard layout
                //set_layout( layout + 1 );
                break;

              case SDLK_h: // Toggle help
                draw_help( help_shown = !help_shown );
                break;

              default: // WASD control scheme
                if( event.key.keysym.sym == keymap[ KM_LEFT ] ) {
                  ctrl.ctrl.kb |= KB_LEFT;
                } else if( event.key.keysym.sym == keymap[ KM_RIGHT ] ) {
                  ctrl.ctrl.kb |= KB_RIGHT;
                } else if( event.key.keysym.sym == keymap[ KM_UP ] ) {
                  ctrl.ctrl.kb |= KB_UP;
                } else if( event.key.keysym.sym == keymap[ KM_DOWN ] ) {
                  ctrl.ctrl.kb |= KB_DOWN;
                } else if( state == STATE_STREAMING && keyboard_binds[ event.key.keysym.sym ] != NULL ) {
                  plug = keyboard_binds[ event.key.keysym.sym ]; // Notify plugin
                  if( plug->keyboard ) plug->keyboard( E_KEYDOWN, event.key.keysym.sym, unicode_ascii( event.key.keysym.unicode ) );
                }
                break;

            }
          } else if( state == STATE_STREAMING ) {
            plug = keyboard_hook;
            if( plug->keyboard ) plug->keyboard( E_KEYDOWN, event.key.keysym.sym, unicode_ascii( event.key.keysym.unicode ) );
          }
          break;
        case SDL_KEYUP:
          if( state == STATE_STREAMING && keyboard_hook == NULL ) {
            // WASD control scheme
            if( event.key.keysym.sym == keymap[ KM_LEFT ] ) {
              ctrl.ctrl.kb &= ~KB_LEFT;
            } else if( event.key.keysym.sym == keymap[ KM_RIGHT ] ) {
              ctrl.ctrl.kb &= ~KB_RIGHT;
            } else if( event.key.keysym.sym == keymap[ KM_UP ] ) {
              ctrl.ctrl.kb &= ~KB_UP;
            } else if( event.key.keysym.sym == keymap[ KM_DOWN ] ) {
              ctrl.ctrl.kb &= ~KB_DOWN;
            } else if( keyboard_binds[ event.key.keysym.sym ] != NULL ) {
              plug = keyboard_binds[ event.key.keysym.sym ]; // Notify plugin
              if( plug->keyboard ) plug->keyboard( E_KEYUP, event.key.keysym.sym, unicode_ascii( event.key.keysym.unicode ) );
            }
          } else if( keyboard_hook ) {
            plug = keyboard_hook; // Notify plugin
            if( plug->keyboard ) plug->keyboard( E_KEYUP, event.key.keysym.sym, unicode_ascii( event.key.keysym.unicode ) );
          }
          break;
      }
    }

    // Delay 1/CLIENT_RPS seconds, constantly correct for processing overhead
    time_diff = SDL_GetTicks() - time_target;
    //printf( "decode: %i, time: %i, diff: %i, ", do_decode, SDL_GetTicks(), time_diff );
    if( time_diff > 1000 ) {
      printf( "WARN: Cannot keep up with the desired RPS\n" );
      time_target = SDL_GetTicks();
      time_diff = ( 1000 / CLIENT_RPS );
    }
    time_target += 1000 / CLIENT_RPS;
    //printf( "target: %i, delay: %i\n", time_target, MAX( 0, ( 1000 / CLIENT_RPS ) - time_diff ) );
    SDL_Delay( MAX( 0, ( 1000 / CLIENT_RPS ) - time_diff ) );

  }

  // Send QUIT
  comm_send( pkt_quit, 4 );

  // Clean up
  sprites_free();
  avcodec_close( pCodecCtx );
  SDL_DestroyMutex( trust_mx );
  SDL_FreeSurface( frame );
  joystick_close();
  SDL_Quit();

  printf( "RoboCortex [info]: KTHXBYE!\n" );

  exit( EXIT_OK );
}
