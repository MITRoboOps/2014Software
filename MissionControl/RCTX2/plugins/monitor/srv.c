#include "srv.h" // This is a server plugin

static pluginclient_t  monitor;         // Plugin descriptor
static  pluginhost_t  *host;            // RoboCortex descriptor
static int active = 255;
static int active2 = 255;
// Handles packets received from client plugin
static void process_data( void* p_data, unsigned char size ) {
    char data[8];
    memcpy(&data, p_data, 2 );
    //data[size] = 0;
    active = data[0] - '0';
    active2 = data[1] - '0';
    //printf("Active CAM1=%d. CAM2=%d\n", active, active2);
}

// Tick: updates cameras
static void tick() {
  int n;
  float f;
  SDL_Rect src, dst;
  int w, h, e;
  int block = 0;

  for( n = 0; n < host->cap_count; n++ ) {
    if( n != active && n != active2 ) {
        host->cap_get( n, &w, &h, &e, &src, &dst );
        dst.x = ( n % 2 ) * ( host->stream_w / 2 );
        dst.y = ( n / 2 ) * ( host->stream_h / 2 );
        dst.w = ( host->stream_w / 2 );
        dst.h = ( host->stream_h / 2 );
        host->cap_set( n, CAP_ENABLE, &src, &dst );
    }
  }

  //  printf("active %d. host->cap_count %d. block %d.", active, host->cap_count, block);
  if( active >= 0 && active < host->cap_count && !block ) {
    host->cap_get( active, &w, &h, &e, &src, &dst );
    dst.x = 0;
    dst.y = 0;
    dst.w = host->stream_w/2;
    dst.h = host->stream_h;
    host->cap_set( active, CAP_ENABLE, &src, &dst );
    host->cap_zorder( active, host->cap_count );
  }
  if( active2 >= 0 && active2 < host->cap_count && !block ) {
    host->cap_get( active2, &w, &h, &e, &src, &dst );
    dst.x = host->stream_w/2;
    dst.y = 0;
    dst.w = host->stream_w/2;
    dst.h = host->stream_h;
    host->cap_set( active2, CAP_ENABLE, &src, &dst );
    host->cap_zorder( active2, host->cap_count );
  }
}

static void init() {
  int n;
  SDL_Rect src, dst;
  int w, h, e;
  for( n = 0; n < host->cap_count; n++ ) {
    host->cap_get( n, &w, &h, &e, &src, &dst );
        dst.x = ( n % 2 ) * ( host->stream_w / 2 );
        dst.y = ( n / 2 ) * ( host->stream_h / 2 );
        dst.w = ( host->stream_w / 2 );
        dst.h = ( host->stream_h / 2 );
        host->cap_set( n, CAP_ENABLE, &src, &dst );
  }
}

// Sets up the plugin descriptor
pluginclient_t *monitor_open( pluginhost_t *p_host ) {
  memcpy( &monitor.ident, "MON9", 4 );
  host = p_host;
  monitor.init       = init;
  monitor.tick       = tick;
  monitor.recv       = process_data;
  return( &monitor );
}
