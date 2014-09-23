#include <stdio.h>
#include <signal.h>
#include <SDL/SDL.h>
#include <libswscale/swscale.h>
#include <x264.h>
#include "oswrap.h"
#include "robocortex.h"
#include "speech.h"
#include "plugins/srv.h"
#include "sdl_console.h"

// Plugins
#define MAX_PLUGINS           16
extern pluginclient_t *kiwiray_open( pluginhost_t* );
extern pluginclient_t *monitor_open( pluginhost_t* );
extern pluginclient_t *ipv4udp_open( pluginhost_t* );

// Save the stream
//#define SAVE_STREAM            "server.h264"

// Protocol
#define MAX_CLIENTS           10 // Max number of clients allowed in the quuee

// Capture (device may not be capable and return another size)
#define CAP_SOURCES           16 // Max number of capture sources

// Stream default size
#define STREAM_WIDTH         320 // Width of streamed video
#define STREAM_HEIGHT        240 // Height of streamed video

// Default FPS
#define FPS                   25 // FPS of streamed video (also requested capture FPS)

// Default timeouts (in frames, see fps)
#define TIMEOUT_CONNECTION   100 // Before connection is closed if no data has arrived
#define TIMEOUT_CONTROL     7500 // Before control session is ended
#define TIMEOUT_TRUST          8 // Before retransmitting trusted packets
#define TIMEOUT_GLITCH         2 // Before robot stops moving if a connection glitches/is lost

// Exit code list
enum exitcode_e {
  EXIT_OK,
  EXIT_NETWORK,
  EXIT_SOCKET,
  EXIT_BIND,
  EXIT_CAPTURE,
  EXIT_SWSCALE,
  EXIT_PICTURE,
  EXIT_AUDIO,
  EXIT_NOSOURCE,
  EXIT_CONFIG,
  EXIT_MALLOC
};

// Client data
struct client_t {
  remote_t           remote;
  int                timeout;
  int                timer;
  int                glitch;
  unsigned char      trust_cli;
  unsigned char      trust_srv;
  struct client_t   *prev;
  struct client_t   *next;
  int                index;
  ctrl_data_t        ctrl;
  int                got_first;
  ctrl_t             last;
  ctrl_t             diff;
  unsigned char      trust_data;
};
typedef struct client_t client_t;

// Capture setting
typedef struct {
  int                enable;
  char               device[ CFG_VALUE_MAX_SIZE ];
  int                dev;
  int                w, h;
  int                z;
  SDL_Rect           src, dst;
  uint8_t           *data;
  struct SwsContext *swsCtx;
  int                load_order;
  int                index;
} capture_t;

// Locals
static               int  quit     = 0; // Time to quit (SIGINT etc.)
static               int  do_intra = 0; // Time to intra-refresh (New client connected)

// Clients linked-list array
static               int  max_clients = MAX_CLIENTS;
static          client_t *clients;
static          client_t *client_first = NULL;
static          client_t *client_last  = NULL;
static         SDL_mutex *client_mx;
static               int  direct;

// Trusted data linked_list & mutex
static      linked_buf_t *trust_first = NULL;
static      linked_buf_t *trust_last = NULL;
static               int  trust_timeout;
static         SDL_mutex *trust_mx;

// Packet types
static              char  pkt_data[ 4 ] = "DATA";
static              char  pkt_helo[ 4 ] = "HELO";
static              char  pkt_time[ 4 ] = "TIME";
static              char  pkt_ctrl[ 4 ] = "CTRL";
static              char  pkt_lost[ 4 ] = "LOST";
static              char  pkt_full[ 4 ] = "FULL";
static              char  pkt_quit[ 4 ] = "QUIT";

// Configuration
static           char  config_default[] = "srv.rc"; // Default configuration file

// Capture
static         capture_t  cap[ CAP_SOURCES ];
static               int  cap_count = -1;
static         SDL_mutex *cap_mx;

// Encoding and conversion
static               int  stream_w = STREAM_WIDTH, stream_h = STREAM_HEIGHT, fps = FPS;
static               int  stream_stride;
static            x264_t *encoder;
static    x264_picture_t  pic_in, pic_out;
static     unsigned char *pic_rgb24;
static struct SwsContext *swsCtx;

// Receive data mutex TODO: replace with buffering system and semaphore?
static         SDL_mutex *receive_mx;

// Timeouts
static               int  timeout_connection = TIMEOUT_CONNECTION;
static               int  timeout_control = TIMEOUT_CONTROL;
static               int  timeout_trust = TIMEOUT_TRUST;
static               int  timeout_glitch = TIMEOUT_GLITCH;

// Plugins
static      pluginhost_t  host;
static    pluginclient_t *plug;
static         SDL_mutex *plug_mx;
static    pluginclient_t *plugs[ MAX_PLUGINS ];
static               int  plugs_count;

// Speech
static              char  sysvoice[ 256 ] = "default";
#ifdef USE_SAM
static              char  voice[ 1 ][ 256 ] = { "SAM:SAM" };
static               int  voice_count = 1;
#else
static              char  voice[ 10 ][ 256 ];
static               int  voice_count = 0;
#endif
static               int  voice_index = 0;
static              char  readytext[ 80 ] = "";

/* == TRUSTED COMMUNICATIONS ==================================================================== */

// Queues a packet for trusted (non-lossy) transmission
static void trust_queue( uint32_t ident, void* data, unsigned char size ) {
  linked_buf_t *p_trust;
  p_trust = malloc( sizeof( linked_buf_t ) );
  // TODO: check
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
static void trust_handler( client_t *p_client, char* data, int size ) {
  unsigned char n;
  int pid;
  uint32_t ident;
  unsigned char len;
  if( size == 0 ) return;
  data[ size ] = 0;
  p_client->trust_cli++;

  // Pass data to plugins
  while( size > 5 ) {
    ident = *( uint32_t* )data; data += 4;
    len = *data++;
    size -= 5;
    if( size >= len ) {
      // plugin->recv
      SDL_mutexP( plug_mx );
      for( pid = 0; pid < MAX_PLUGINS && ( plug = plugs[ pid ] ) != NULL; pid++ ) {
        if( plug->ident == ident ) {
          if( plug->recv ) plug->recv( data, len );
        }
      }
      SDL_mutexV( plug_mx );
    }
    size -= len;
  }
}

// Clear trusted queue
static void trust_clear() {
  linked_buf_t *p_trust;
  SDL_mutexP( trust_mx );
  while( trust_first ) {
    p_trust = trust_first->next;
    free( trust_first );
    trust_first = p_trust;
  }
  SDL_mutexV( trust_mx );
  trust_timeout = 0;
}

/* == CONFIGURATION ============================================================================= */

static int config_set( char *value, char *token ) {
  int n;
  if( token != NULL ) {
    if( strcmp( token, "width" ) == 0 ) {
      stream_w = atoi( value );
    } else if( strcmp( token, "height" ) == 0 ) {
      stream_h = atoi( value );
    } else if( strcmp( token, "fps" ) == 0 ) {
      fps = atoi( value );
    } else if( strcmp( token, "queue" ) == 0 ) {
      max_clients = atoi( value );
    } else if( strcmp( token, "timeout_connection" ) == 0 ) {
      timeout_connection = atoi( value );
    } else if( strcmp( token, "timeout_control" ) == 0 ) {
      timeout_control = atoi( value );
    } else if( strcmp( token, "timeout_trust" ) == 0 ) {
      timeout_trust = atoi( value );
    } else if( strcmp( token, "timeout_glitch" ) == 0 ) {
      timeout_glitch = atoi( value );
#ifndef USE_SAM
    } else if( strcmp( token, "sysvoice" ) == 0 ) {
      if( strlen( value ) >= 256 ) printf( "Config [warning]: sysvoice length exceeded\n" );
      else strcpy( sysvoice, value );
    } else if( strcmp( token, "voice" ) == 0 ) {
      if( voice_count == 10 ) printf( "Config [warning]: number of voices exceeded\n" );
      else if( strlen( value ) >= 256 ) printf( "Config [warning]: voice length exceeded\n" );
      else if( strchr( value, ':' ) == NULL ) printf( "Config [warning]: malformed voice identifier\n" );
      else strcpy( voice[ voice_count++ ], value );
#endif        
    } else if( strcmp( token, "readytext" ) == 0 ) {
      if( strlen( value ) >= 80 ) printf( "Config [warning]: readytext length exceeded" );
      else strcpy( readytext, value );
    } else if( strcmp( token, "device" ) == 0 ) {
      if( cap_count >= ( CAP_SOURCES - 1 ) ) printf( "Config [warning]: too many capture sources.\n" );
      else {
        cap_count++;
        cap[ cap_count ].enable = 1;
        cap[ cap_count ].z = cap_count;
        strcpy( cap[ cap_count ].device, value );
      }
    } else if( strcmp( token, "cap_w" ) == 0 ) {
      if( cap_count < 0 ) printf( "Config [warning]: cap_x outside device section\n" );
      else {
        cap[ cap_count ].w = atoi( value );
        rect( &cap[ cap_count ].src, 0, cap[ cap_count ].src.y, cap[ cap_count ].w, cap[ cap_count ].src.h );
        rect( &cap[ cap_count ].dst, 0, cap[ cap_count ].dst.y, stream_w, cap[ cap_count ].dst.h );
      }
    } else if( strcmp( token, "cap_h" ) == 0 ) {
      if( cap_count < 0 ) printf( "Config [warning]: cap_h outside device section\n" );
      else {
        cap[ cap_count ].h = atoi( value );
        rect( &cap[ cap_count ].src, cap[ cap_count ].src.x, 0, cap[ cap_count ].src.w, cap[ cap_count ].h );
        rect( &cap[ cap_count ].dst, cap[ cap_count ].dst.x, 0, cap[ cap_count ].dst.w, stream_h );
      }
    } else if( strcmp( token, "src_x" ) == 0 ) {
      if( cap_count < 0 ) printf( "Config [warning]: src_x outside device section\n" );
      else cap[ cap_count ].src.x = atoi( value );
    } else if( strcmp( token, "src_y" ) == 0 ) {
      if( cap_count < 0 ) printf( "Config [warning]: src_y outside device section\n" );
      else cap[ cap_count ].src.y = atoi( value );
    } else if( strcmp( token, "src_w" ) == 0 ) {
      if( cap_count < 0 ) printf( "Config [warning]: src_w outside device section\n" );
      else cap[ cap_count ].src.w = atoi( value );
    } else if( strcmp( token, "src_h" ) == 0 ) {
      if( cap_count < 0 ) printf( "Config [warning]: src_h outside device section\n" );
      else cap[ cap_count ].src.h = atoi( value );
    } else if( strcmp( token, "dst_x" ) == 0 ) {
      if( cap_count < 0 ) printf( "Config [warning]: dst_x outside device section\n" );
      else cap[ cap_count ].dst.x = atoi( value );
    } else if( strcmp( token, "dst_y" ) == 0 ) {
      if( cap_count < 0 ) printf( "Config [warning]: dst_y outside device section\n" );
      else cap[ cap_count ].dst.y = atoi( value );
    } else if( strcmp( token, "dst_w" ) == 0 ) {
      if( cap_count < 0 ) printf( "Config [warning]: dst_w outside device section\n" );
      else cap[ cap_count ].dst.w = atoi( value );
    } else if( strcmp( token, "dst_h" ) == 0 ) {
      if( cap_count < 0 ) printf( "Config [warning]: dst_h outside device section\n" );
      else cap[ cap_count ].dst.h = atoi( value );
    } else if( strcmp( token, "load_order" ) == 0 ) {
      if( cap_count < 0 ) printf( "Config [warning]: load_order outside device section\n" );
      else cap[ cap_count ].load_order = atoi( value );
    } else if( strcmp( token, "plugin" ) == 0 ) {
      return( 1 );
    } else printf( "Config [warning]: unknown entry %s\n", token );
  } else {
    printf( "Config [info]: stream is %ix%ix%ifps\n", stream_w, stream_h, fps );
    for( n = 0; n <= cap_count; n++ ) {
      printf( "Config [info]: capture %i:%s is %ix%i, %i:%ix%i:%i -> %i:%ix%i:%i\n",
        n, cap[ n ].device, cap[ n ].w, cap[ n ].h,
        cap[ n ].src.x, cap[ n ].src.w, cap[ n ].src.y, cap[ n ].src.h,
        cap[ n ].dst.x, cap[ n ].dst.w, cap[ n ].dst.y, cap[ n ].dst.h
      );
    }
    cap_count = n;
  }
  return( 0 );
}

/* == CLIENTS MANAGEMENT ======================================================================== */

// Add client, return index or -1 if queue is full
static client_t *clients_add( remote_t *remote ) {
  int n, pid;
  client_t *p_ret = NULL;
  // Iterate client table
  SDL_mutexP( client_mx );
  if( direct ) {
    if( clients->remote.size == remote->size ) {
      if( memcmp( clients->remote.addr, remote->addr, remote->size ) == 0 ) {
        if( clients->remote.addr ) free( clients->remote.addr );
        clients->remote.addr = malloc( remote->size );
        // TODO: check
        memcpy( clients->remote.addr, remote->addr, remote->size );
        clients->remote.size = remote->size;
        clients->remote.handler = remote->handler;
        clients->trust_cli = 0xFF;
        clients->trust_srv = 0x00;
        clients->got_first = 0;
        clients->timeout = timeout_connection;
        do_intra = 1;
        // plugin->connected( 1 )
        SDL_mutexP( plug_mx );
        for( pid = 0; pid < MAX_PLUGINS && ( plug = plugs[ pid ] ) != NULL; pid++ )
          if( plug->connected ) plug->connected( 1 );
        SDL_mutexV( plug_mx );
      }
    }
    if( client_first == NULL ) {
      client_first = clients;
      host.ctrl = &client_first->ctrl.ctrl;
      host.diff = &client_first->diff;
    }
    p_ret = clients;
  } else {
    for( n = 0; n < max_clients; n++ ) {
    	// Find free client entry
      if( !clients[ n ].timeout ) {
      	// Add client to table
        printf( "RoboCortex [info]: Client %i connected\n", n );
        if( clients[ n ].remote.addr ) free( clients[ n ].remote.addr );
        clients[ n ].remote.addr = malloc( remote->size );
        // TODO: check
        memcpy( clients[ n ].remote.addr, remote->addr, remote->size );
        clients[ n ].remote.size = remote->size;
        clients[ n ].remote.handler = remote->handler;
        clients[ n ].next = NULL;
        clients[ n ].prev = client_last;
        clients[ n ].trust_cli = 0xFF;
        clients[ n ].trust_srv = 0x00;
        clients[ n ].got_first = 0;
        clients[ n ].timeout = timeout_connection;
        clients[ n ].timer   = timeout_control;
        if( client_first ) {
          client_last->next = &clients[ n ];
        } else {
          do_intra = 1; // Intra-refresh needed
          trust_clear();
          client_first = &clients[ n ];
          host.ctrl = &client_first->ctrl.ctrl;
          host.diff = &client_first->diff;
          // plugin->connected( 1 )
          SDL_mutexP( plug_mx );
          for( pid = 0; pid < MAX_PLUGINS && ( plug = plugs[ pid ] ) != NULL; pid++ )
            if( plug->connected ) plug->connected( 1 );
          SDL_mutexV( plug_mx );
        }
        client_last = &clients[ n ];
        p_ret = &clients[ n ];
        break;
      }
    }
  }
  SDL_mutexV( client_mx );
  return( p_ret );
}

// Find client index
static client_t *clients_find( remote_t *remote ) {
  int n;
  client_t *p_ret = NULL;
  SDL_mutexP( client_mx );
  for( n = 0; n < max_clients; n++ ) {
    if( clients[ n ].timeout ) {
      if( clients[ n ].remote.size == remote->size ) {
        if( memcmp( clients[ n ].remote.addr, remote->addr, remote->size ) == 0 ) {
          p_ret = &clients[ n ];
          break;
        }
      }
    }
  }
  SDL_mutexV( client_mx );
  if( direct ) p_ret = clients_add( remote );
  return( p_ret );
}

// Calculates control differentals
static void clients_diff( client_t *p_client ) {
  p_client->diff.mx = p_client->ctrl.ctrl.mx - p_client->last.mx;
  p_client->diff.my = p_client->ctrl.ctrl.my - p_client->last.my;
  p_client->diff.kb = p_client->ctrl.ctrl.kb ^ p_client->last.kb;
  memcpy( &p_client->last, &p_client->ctrl.ctrl, sizeof( ctrl_t ) );
}

// Calculates queue time for specific client
static char * queue_time( char buf[], int offset, client_t *p_client ) {
  client_t *cli = client_first;
  int total = 0;
  while( cli ) {
    if( cli == p_client ) break;
    total += cli->timer;
    cli = cli->next;
  }
  *( int* )&buf[ offset ] = total;
  return( buf );
}

// Count down all client timers, kill active client if it's timer reaches zero
static void clients_tick() {
  int n, pid;
  // Count down timeouts
  SDL_mutexP( client_mx );
  for( n = 0; n < max_clients; n++ ) {
  	if( clients[ n ].timeout ) {
  		clients[ n ].timeout--;
  		if( clients[ n ].timeout == 0 ) {
        printf( "RoboCortex [info]: Client %i disconnected (ping timeout)\n", n );
  			if( clients[ n ].prev ) clients[ n ].prev->next = clients[ n ].next;
  			if( clients[ n ].next ) clients[ n ].next->prev = clients[ n ].prev;
  		}
  	}
 	}
  if( client_first ) {
  	// Count down timer
    if( client_first->glitch ) client_first->glitch--;
    if( --client_first->timer == 0 || client_first->timeout == 0 ) {
    	// Time for client switch
      printf( "RoboCortex [info]: Client disconnected (time up)\n" );
      client_first = client_first->next;
      host.ctrl = &client_first->ctrl.ctrl;
      host.diff = &client_first->diff;
      // plugin->connected( 0 )
      SDL_mutexP( plug_mx );
      for( pid = 0; pid < MAX_PLUGINS && ( plug = plugs[ pid ] ) != NULL; pid++ )
        if( plug->connected ) plug->connected( 0 );
      SDL_mutexV( plug_mx );
      if( client_first ) {
        // plugin->connected( 1 )
        SDL_mutexP( plug_mx );
        for( pid = 0; pid < MAX_PLUGINS && ( plug = plugs[ pid ] ) != NULL; pid++ )
          if( plug->connected ) plug->connected( 1 );
        SDL_mutexV( plug_mx );
      	client_first->prev = NULL;
	      do_intra = 1; // Intra-refresh needed
      }
	    trust_clear();
    }
    if( client_first ) if( client_first->timer < 0 ) client_first->timer = 0;
  }
  SDL_mutexV( client_mx );
}

/* == COMMUNICATIONS ============================================================================ */

// Processes a data packet
static void comm_recv( char *buffer, int size, remote_t *remote ) {
/*
  // JonasB debug code
  printf("RoboCortex [debug]: Recevied packet: ");
  int i;
  for (i = 0; i != size; i++) {
    printf("%02x", buffer[i]);
  }
  printf("\n");
*/
  client_t *p_client = clients_find( remote );
  SDL_mutexP( receive_mx );
  if( p_client ) {
    if( size >= 4 ) {
      if( memcmp( buffer, pkt_helo, 4 ) == 0 ) {
        // Re-send HELO+version+time
        buffer[ 4 ] = CORTEX_VERSION;
        ( ( pluginclient_t* )( remote->handler ) )->comm_send( queue_time( buffer, 5, p_client ), 5 + sizeof( int ), remote );
      } else if( memcmp( buffer, pkt_time, 4 ) == 0 ) {
        // Send TIME+time
        ( ( pluginclient_t* )( remote->handler ) )->comm_send( queue_time( buffer, 4, p_client ), 4 + sizeof( int ), remote );
        SDL_mutexP( client_mx );
        p_client->timeout = timeout_connection;
        SDL_mutexV( client_mx );
      } else if( memcmp( buffer, pkt_quit, 4 ) == 0 ) {
        // Abort connection
        SDL_mutexP( client_mx );
        p_client->timeout = 0;
        SDL_mutexV( client_mx );
      } else if( memcmp( buffer, pkt_ctrl, 4 ) == 0 ) {
        // Copy control data
        if( size >= 4 + sizeof( ctrl_data_t ) ) {
          SDL_mutexP( client_mx );
          p_client->timeout = timeout_connection;
          p_client->glitch = timeout_glitch;
          SDL_mutexV( client_mx );
          memcpy( &p_client->ctrl, buffer + 4, sizeof( ctrl_data_t ) );
          // Initial control data, reset diff
          if( !p_client->got_first ) {
            p_client->got_first = 1;
            memcpy( &p_client->last, &p_client->ctrl.ctrl, sizeof( ctrl_t ) );
          }
          // Check if outgoing trusted data recieved, free trusted buffers
          SDL_mutexP( trust_mx );
          if( trust_first ) {
            if( p_client->ctrl.trust_srv == p_client->trust_srv ) {
              linked_buf_t* p_trust = trust_first;
              trust_first = trust_first->next;
              p_client->trust_srv++;
              trust_timeout = 0;
              free( p_trust );
            }
          }
          SDL_mutexV( trust_mx );
          // Handle incoming trusted data
          if( ( ( p_client->trust_cli + 1 ) & 0xFF ) == p_client->ctrl.trust_cli ) {
            trust_handler( p_client, buffer + 4 + sizeof( ctrl_data_t ), size - 4 - sizeof( ctrl_data_t ) );
          }
        }
      }
    }
  } else {
    // Client unknown
    if( size >= 4 ) {
      if( memcmp( buffer, pkt_helo, 4 ) == 0 ) {
        // Handshake, add
        p_client = clients_add( remote );
        if( p_client ) {
          // Connection accepted, send HELO+version+time
          buffer[ 4 ] = CORTEX_VERSION;
          ( ( pluginclient_t* )( remote->handler ) )->comm_send( queue_time( buffer, 5, p_client ), 5 + sizeof( int ), remote );
        } else {
          // Server is full, send FULL
          ( ( pluginclient_t* )( remote->handler ) )->comm_send( pkt_full, 4, remote );
        }
      } else {
        // Unknown connection, send LOST
        ( ( pluginclient_t* )( remote->handler ) )->comm_send( pkt_lost, 4, remote );
      }
    }
  }
  SDL_mutexV( receive_mx );
}

/* == CAPTURE SCALING & CONVERSION ============================================================== */

static void cap_context( int n ) {
  SDL_mutexP( cap_mx );
  if( cap[ n ].swsCtx ) sws_freeContext( cap[ n ].swsCtx );
  cap[ n ].swsCtx = sws_getContext( cap[ n ].src.w, cap[ n ].src.h, PIX_FMT_RGB24, cap[ n ].dst.w, cap[ n ].dst.h, PIX_FMT_RGB24, SWS_FAST_BILINEAR, NULL, NULL, NULL );
  SDL_mutexV( cap_mx );
}

// Convert, crop, scale and blit all RGB24 capture sources onto YUV420P destination
static void cap_process() {
  uint8_t* r_dst;
  const uint8_t *r_src;
  int src_stride;
  int z, n;
  memset( pic_rgb24, 0, stream_h * stream_stride );
  for( z = 0; z < cap_count; z++ ) {
    for( n = 0; n < cap_count; n++ ) {
      if( cap[ n ].z == z ) {
        SDL_mutexP( cap_mx );
        if( cap[ n ].enable ) {
          src_stride = cap[ n ].w * 3;
          r_src = cap[ n ].data + ( src_stride * cap[ n ].src.y ) + ( cap[ n ].src.x * 3 );
          r_dst = pic_rgb24 + ( cap[ n ].dst.y * stream_stride ) + cap[ n ].dst.x * 3;
          sws_scale( cap[ n ].swsCtx, &r_src, &src_stride, 0, cap[ n ].src.h, &r_dst, &stream_stride );
        }
        SDL_mutexV( cap_mx );
        break;
      }
    }
  }
}

/* == PLUGIN SYSTEM ============================================================================= */

// Plugin helpers
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

static void plug_send( void* data, unsigned char size ) {
  trust_queue( plug->ident, data, size );
}

static void plug_capget( int dev, int *w, int *h, int *e, SDL_Rect *src, SDL_Rect *dst ) {
  *w = cap[ dev ].w;
  *h = cap[ dev ].h;
  SDL_mutexP( cap_mx );
  *e = cap[ dev ].enable;
  memcpy( src, &cap[ dev ].src, sizeof( SDL_Rect ) );
  memcpy( dst, &cap[ dev ].dst, sizeof( SDL_Rect ) );
  SDL_mutexV( cap_mx );
}

static void plug_capset( int dev, int e, SDL_Rect *src, SDL_Rect *dst ) {
  int changed = 0;
  cap[ dev ].enable = ( e == CAP_ENABLE ? 1 : ( e == CAP_DISABLE ? 0 : dev[ cap ].enable != ( e == CAP_TOGGLE ? 1 : 0 ) ) );
  if( src || dst ) {
    SDL_mutexP( cap_mx );
    if( src ) if( memcmp( &cap[ dev ].src, src, sizeof( SDL_Rect ) ) != 0 ) changed = 1;
    if( dst ) if( memcmp( &cap[ dev ].dst, dst, sizeof( SDL_Rect ) ) != 0 ) changed = 1;
    if( changed ) {
      if( src ) memcpy( &cap[ dev ].src, src, sizeof( SDL_Rect ) );
      if( dst ) memcpy( &cap[ dev ].dst, dst, sizeof( SDL_Rect ) );
      cap_context( dev );
    }
    SDL_mutexV( cap_mx );
  }
}

static void plug_capz( int dev, int z ) {
  int n;
  z = MIN( MAX( z, 0 ), cap_count - 1 );
  if( cap[ dev ].z != z ) {
    for( n = 0; n < cap_count; n++ ) if( n != dev && cap[ n ].z > cap[ dev ].z ) cap[ n ].z--;
    for( n = 0; n < cap_count; n++ ) if( n != dev && cap[ n ].z >= z ) cap[ n ].z++;
    cap[ dev ].z = z;
  }
}

static int plug_cfg( char* dst, char* req_token ) {
  return( config_plugin( plug->ident, dst, req_token ) );
}

static void speak_text( char* text ) {
  speech_queue( strchr( voice[ voice_index ], ':' ) + 1, text );
}

void speak_viseme( uint8_t viseme ) {
  int pid;
  SDL_mutexP( plug_mx );
  for( pid = 0; pid < MAX_PLUGINS && ( plug = plugs[ pid ] ) != NULL; pid++ )
    if( plug->viseme ) plug->viseme( viseme );
  SDL_mutexV( plug_mx );
}

static char* speak_voice( int index ) {
    if( index >= 0 ) {
      if( index < 10 ) {
        voice_index = index;
      }
    } else if( index == -1 ) {
      if( ++voice_index >= voice_count ) voice_index = 0;
    } else if( index == -2 ) {
      if( --voice_index < 0 ) voice_index = voice_count - 1;
    }
    return( voice[ voice_index ] );
}

static void load_plugins() {
  int pid;
  host.thread_start = plug_thrstart;
  host.thread_stop  = plug_thrstop;
  host.thread_delay = plug_thrdelay;
  host.speak_text   = speak_text;
  host.speak_voice  = speak_voice;
  host.client_send  = plug_send;
  host.cfg_read     = plug_cfg;
//  host.cap_enable   = plug_cap;
  host.cap_set      = plug_capset;
  host.cap_get      = plug_capget;
  host.cap_zorder   = plug_capz;
  host.comm_recv    = comm_recv;
  printf( "RoboCortex [info]: Loading plugins...\n" );
  // Load plugins
  plugs[ plugs_count++ ] = kiwiray_open( &host );
  plugs[ plugs_count++ ] = monitor_open( &host );
  plugs[ plugs_count++ ] = ipv4udp_open( &host );
  printf( "RoboCortex [info]: Initializing plugins...\n" );
  // plugin->init
  SDL_mutexP( plug_mx );
  for( pid = 0; pid < MAX_PLUGINS && ( plug = plugs[ pid ] ) != NULL; pid++ ) {
    if( config_plugin( plug->ident, NULL, NULL ) ) {
      if( plug->init ) plug->init();
    } else {
      memset( plug, 0, sizeof( pluginclient_t ) );
    }
  }
  SDL_mutexV( plug_mx );
  printf( "RoboCortex [info]: Plugins loaded and initialized\n" );
}

static void unload_plugins() {
  int pid;
  // plugin->close
  SDL_mutexP( plug_mx );
  for( pid = 0; pid < MAX_PLUGINS && ( plug = plugs[ pid ] ) != NULL; pid++ )
    if( plug->close ) plug->close();
  SDL_mutexV( plug_mx );
}

/* == APPLICATION CONTROL ======================================================================= */

static void terminate( int z ) {
  printf( "\nRoboCortex [info]: SIGINT received, shutting down...\n\n" );
  quit = 1;
}

// Cleanup
void close_message() {
  printf( "\nRoboCortex [info]: KTHXBYE!\n" );
}

void encoder_free() {
  x264_encoder_close( encoder );
}

void i420_free() {
  x264_picture_clean( &pic_in );
}

void rgb24_free() {
  free( pic_rgb24 );
}

void mutex_free() {
  SDL_DestroyMutex( cap_mx );
  SDL_DestroyMutex( trust_mx );
  SDL_DestroyMutex( client_mx );
  SDL_DestroyMutex( receive_mx );
  SDL_DestroyMutex( plug_mx );
}

void sws_free() {
  int n;
  for( n = 0; n < cap_count; n++ ) {
    if( cap[ n ].swsCtx != NULL ) sws_freeContext( cap[ n ].swsCtx );
    cap[ n ].swsCtx = NULL;
  }
}

void ctx_free() {
  sws_freeContext( swsCtx );
}

void clients_free() {
  int n;
  for( n = 0; n < max_clients; n++ ) {
    if( clients[ n ].remote.addr ) free( clients[ n ].remote.addr );
  }
  free( clients );
}

/* == MAIN THREAD =============================================================================== */

int main( int argc, char *argv[] ) {
  int            n, pid;
	int            cap_w, cap_h;
  x264_param_t   param;
  x264_nal_t    *nals;
  int            i_nals;
  int            frame_size;
  char           p_buffer[ 65536 ] __attribute__ ((aligned));
  unsigned int   i_buffer;
  int            pl, pm, pt = 0;
  int            nalc = 0, nalb = 0;
  disp_data_t    disp;
  int            temp;
  int            l_min, l_max, l;
  Uint32         time_target;
  Sint32         time_diff;
  FILE          *cf;
#ifdef SAVE_STREAM
  FILE          *sf;
#endif

  printf( "RoboCortex [info]: OHAI!\n\n" );
  atexit( close_message );
	signal( SIGINT, terminate );

  // Read configuration file
  config_rc = ( argc > 1 ? argv[ 1 ] : config_default );
  config_parse( config_set );

  // Validate configuration
  if( cap_count == 0 ) {
    printf( "RoboCortex [error]: No capture sources\n" );
    exit( EXIT_NOSOURCE );
  }
  host.cap_count = cap_count;
  if( max_clients == 0 ) {
    max_clients = 1;
    direct = 1;
    timeout_control = 0;
  }

  // Allocate client memory
  clients = malloc( sizeof( client_t ) * max_clients );
  // TODO: check
  memset( clients, 0, sizeof( client_t ) * max_clients );
  atexit( clients_free );

  // Initialize audio
  if( SDL_Init( SDL_INIT_AUDIO ) == 0 ) {
    atexit( SDL_Quit );
  } else {
    exit( EXIT_AUDIO );
  }

  // Initialize capture sources
  atexit( capture_free );
  l_min = l_max = cap[ 0 ].load_order;
  for( n = 1; n < cap_count; n++ ) {
  	if( cap[ n ].load_order > l_max ) l_max = cap[ n ].load_order;
  	if( cap[ n ].load_order < l_min ) l_min = cap[ n ].load_order;
  }
  for( l = l_min; l < l_max + 1; l++ ) {
	  for( n = 0; n < cap_count; n++ ) {
	  	if( cap[ n ].load_order == l ) {
		    cap_w = cap[ n ].w;
		    cap_h = cap[ n ].h;
		    cap[ n ].index = capture_init( cap[ n ].device, fps, &cap_w, &cap_h );
		    printf("Index: %i\n", cap[n].index );
		    if( cap[ n ].index < 0 ) {
		      fprintf( stderr, "RoboCortex [error]: Unable to open capture device %s\n", cap[ n ].device );
		      exit( EXIT_CAPTURE );
		    }
		    if( cap_w != cap[ n ].w || cap_h != cap[ n ].h ) {
		      fprintf( stderr, "RoboCortex [error]: Capture device %s does not support %ix%i (got %ix%i)\n", cap[ n ].device, cap[ n ].w, cap[ n ].h, cap_w, cap_h);
		      exit( EXIT_CAPTURE );
		    }
		  }
	  }
	}

  // Initialize scaling contexts
  atexit( sws_free );
  for( n = 0; n < cap_count; n++ ) {
    cap_context( n );
    if( cap[ n ].swsCtx == NULL ) {
      printf( "RoboCortex [error]: Unable to initialize conversion context\n" );
      exit( EXIT_SWSCALE );
    }
  }

  // Initialize encoder
  x264_param_default_preset( &param, "medium", "zerolatency" );

  param.i_width   = stream_w;
  param.i_height  = stream_h;
  param.i_fps_num = fps;

  // Settings as explained by http://x264dev.multimedia.cx/archives/249

  x264_param_parse( &param, "slice-max-size", "8192" ); /* Practically disables slicing.
  																											   Slicing is the splitting of frame data into
  																											   a series of NALs, each having a maximum size
  																											   so that they can be transported over
  																											   interfaces that has a limited packet size/MTU */

  x264_param_parse( &param, "vbv-maxrate", "500" );     /* Set VBV mode and max bitrate (kbps).
  																												 VBV is variable bitrate, which means the rate
  																												 will vary depending on how complex the scene
  																												 is at the moment - detail, motion, etc. */

  x264_param_parse( &param, "vbv-bufsize", "30" );		  /* Enable single-frame VBV.
  																												 This will cap all frames so that they only
  																												 contain a maximum amount of information,
  																												 which in turn means that each frame can
  																												 always be sent in one packet and packetss
  																												 will be of a much more unform size. */

  x264_param_parse( &param, "crf", "20" );							/* Constant Rate Factor.
  																												 Tells VBV to target a specific quality. */

  x264_param_parse( &param, "intra-refresh", NULL );		/* Enable intra-refresh.
  																												 Intra-refresh allows single-frame VBV to
  																												 work well by disabling I-frames and
  																												 replacing them with a periodic refresh
  																												 that scans across the image, refreshing
  																												 only a smaller portion each frame.
  																												 I-frames can be seen as a full-frame
  																												 refresh that needs no other data to decode
  																												 the picture, and takes a lot more space
  																												 than P/B (differential) frames. */

  param.b_annexb = 1;																		/* Use Annex-B packaging.
  																											   This appends a marker at the start of
  																											   each NAL unit. */

  param.i_frame_reference = 1;													/* Needed for intra-refresh. */

  x264_param_apply_profile( &param, "high" );						/* Apply HIGH profile.
  																												 Allows for better compression, but needs
  																												 to be supported by the decoder. We use
  																												 FFMPEG, which does support this profile. */

  // Open encoder
  encoder = x264_encoder_open( &param );
  atexit( encoder_free );

  // Allocate RGB24 picture
  stream_stride = stream_w * 3;
  pic_rgb24 = malloc( stream_stride * stream_h );
  // TODO: check & cleanup
  if( pic_rgb24 ) {
    atexit( rgb24_free );
  } else {
    exit( EXIT_MALLOC );
  }
  
  // Allocate I420 picture
  if( x264_picture_alloc( &pic_in, X264_CSP_I420, stream_w, stream_h ) == 0 ) {
    atexit( i420_free );
  } else {
    exit( EXIT_PICTURE );
  }

  // Allocate conversion context 
  swsCtx = sws_getContext( stream_w, stream_h, PIX_FMT_RGB24, stream_w, stream_h, PIX_FMT_YUV420P, SWS_POINT, NULL, NULL, NULL );
  if( swsCtx ) {
    atexit( ctx_free );
  } else {
    exit( EXIT_SWSCALE );
  }

  host.stream_rgb24 = pic_rgb24;
  host.stream_w     = stream_w;
  host.stream_h     = stream_h;

  // Create mutexes
  cap_mx     = SDL_CreateMutex();
  trust_mx   = SDL_CreateMutex();
  client_mx  = SDL_CreateMutex();
  receive_mx = SDL_CreateMutex();
  plug_mx    = SDL_CreateMutex();
  atexit( mutex_free );

  // Load plugins
  load_plugins();
  atexit( unload_plugins );

#ifndef DISABLE_SPEECH
  speech_open();
  atexit( speech_free );
#endif
  speech_queue( sysvoice, readytext );

  printf( "\nRoboCortex [info]: Initialized and ready for connection...\n" );

#ifdef SAVE_STREAM
  sf = fopen( SAVE_STREAM, "wb" );
#endif

  time_target = SDL_GetTicks();

  while( !quit ) {

  	speech_poll();

    // Fetch latest picture from capture devices
    for( n = 0; n < cap_count; n++ ) {
      cap[ n ].data = ( uint8_t * )capture_fetch( cap[ n ].index );
      // plugin->capture
      SDL_mutexP( plug_mx );
      for( pid = 0; pid < MAX_PLUGINS && ( plug = plugs[ pid ] ) != NULL; pid++ )
        if( plug->capture ) plug->capture( n, cap[ n ].w, cap[ n ].h, cap[ n ].data );
      SDL_mutexV( plug_mx );
    }

		// Process and scale sources
		cap_process( pic_in.img.i_stride, pic_in.img.plane );

    // Have client?
    SDL_mutexP( client_mx );
    temp = ( client_first ? 1 : 0 );
    SDL_mutexV( client_mx );

    // Clear motion if control packets are not arriving
    if( temp ) {
      if( client_first->glitch == 0 ) {
        client_first->ctrl.ctrl.kb = 0;
      }
    }

    // Calculate control differentials
    if( temp ) clients_diff( client_first );

    // plugin->tick
    SDL_mutexP( plug_mx );
    for( pid = 0; pid < MAX_PLUGINS && ( plug = plugs[ pid ] ) != NULL; pid++ )
      if( plug->tick ) plug->tick();
    SDL_mutexV( plug_mx );

		// Convert to I420, as explained by http://stackoverflow.com/questions/2940671/how-to-encode-series-of-images-into-h264-using-x264-api-c-c
    sws_scale( swsCtx, ( const uint8_t* const* )&pic_rgb24, &stream_stride, 0, stream_h, pic_in.img.plane, pic_in.img.i_stride );

    // Encode frame
    if( do_intra ) {
      do_intra = 0;
      x264_encoder_intra_refresh( encoder );
    }
    frame_size = x264_encoder_encode( encoder, &nals, &i_nals, &pic_in, &pic_out );

    // Iterate NALs
    pl = 0;
    pm = 0;
    i_buffer = 0;
    for( n = 0; n < i_nals; n++ ) {

      // Track payload sizes
      pl += nals[ n ].i_payload;
      if( nals[ n ].i_payload > pm ) pm = nals[ n ].i_payload;

      // Concatenate into a linear buffer
      memcpy( p_buffer + i_buffer, nals[ n ].p_payload, nals[ n ].i_payload );
      i_buffer += nals[ n ].i_payload;

      // Total counters
      nalc += 1;
      nalb += nals[ n ].i_payload;
    }

#ifdef SAVE_STREAM
    fwrite( p_buffer, 1, i_buffer, sf );
#endif

    // Largest packet
    if( pl > pt ) pt = pl;

    // Client connected?
    if( temp ) {

    	// Send H.264 frame
      ( ( pluginclient_t* )( client_first->remote.handler ) )->comm_send( p_buffer, i_buffer, &client_first->remote );

      // Build DATA packet
      memcpy( p_buffer, "DATA", 4 );
      disp.timer    = client_first->timer;
      disp.trust_cli = client_first->trust_cli;
      disp.trust_srv = client_first->trust_srv;
      memcpy( p_buffer + 4, &disp, sizeof( disp_data_t ) );
      i_buffer = 4 + sizeof( disp_data_t );

      // Append trusted data if any
      SDL_mutexP( trust_mx );
      if( trust_timeout == 0 ) {
        // Trusted data?
        if( trust_first ) {
          memcpy( p_buffer + i_buffer, trust_first->data, trust_first->size );
          i_buffer += trust_first->size;
          trust_timeout = timeout_trust;
        }
      } else {
        trust_timeout--;
      }
      SDL_mutexV( trust_mx );

      // plugin->stream
      SDL_mutexP( plug_mx );
      for( pid = 0; pid < MAX_PLUGINS && ( plug = plugs[ pid ] ) != NULL; pid++ )
        if( plug->stream ) plug->stream( p_buffer, i_buffer );
      SDL_mutexV( plug_mx );

      // Send DATA packet
      ( ( pluginclient_t* )( client_first->remote.handler ) )->comm_send( p_buffer, i_buffer, &client_first->remote );

    } else {
      // plugin->still
      SDL_mutexP( plug_mx );
      for( pid = 0; pid < MAX_PLUGINS && ( plug = plugs[ pid ] ) != NULL; pid++ )
        if( plug->still ) plug->still();
      SDL_mutexV( plug_mx );
    }

    // Delay 1/fps seconds, constantly correct for processing overhead
    time_diff = SDL_GetTicks() - time_target;
    if( time_diff > 1000 / fps ) {
      time_diff = 0;
      time_target = SDL_GetTicks(); // Reset on overflow
      printf( "RoboCortex [warning]: Encoder cannot keep up with desired FPS\n" );
    }
    if( time_diff < 0 ) {
      time_diff = 0;
      printf( "RoboCortex [error]: SDL_Delay returns too fast\n" );
    }
    time_target += 1000 / fps;
    SDL_Delay( ( 1000 / fps ) - time_diff );

    // Tick client timers
    clients_tick();

  }

#ifdef SAVE_STREAM
  fclose( sf );
#endif

  printf( "RoboCortex [info]: NAL units: %i, %i bytes\n", nalc, nalb );
  printf( "RoboCortex [info]: Largest packet: %i\n\n", pt );

  exit( EXIT_OK );
}
