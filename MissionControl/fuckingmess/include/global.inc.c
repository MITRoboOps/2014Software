#include "global.inc.h"
#include "../plugins/cli.h"



// Texts
char  text_contact[] =  "ESTABLISHING CORTEX...";
char  text_error[]   =  "   CONNECTION ERROR   ";
char  text_queued[] =   "  QUEUED FOR CONTROL  ";
char  text_full[] =     " QUEUE IS FULL, SORRY ";
char  text_lost[] =     "  CONNECTION IS LOST  ";
char  text_quit[] =     "   PRESS H FOR HELP   ";
char  text_version[] =  " WRONG CLIENT VERSION ";
char  text_blank[] =    "                      ";
char  text_time[] =     "       00:00:00       ";
char  text_controls[] = "CONNECTED - H FOR HELP, ESC TO QUIT.";
char  text_timeout[] =  "TIME LEFT: 00:00";

// Packet types
char  pkt_h264[ 4 ] = { 0x00, 0x00, 0x00, 0x01 };
char  pkt_data[ 4 ] = "DATA";
char  pkt_helo[ 4 ] = "HELO";
char  pkt_time[ 4 ] = "TIME";
char  pkt_ctrl[ 4 ] = "CTRL";
char  pkt_lost[ 4 ] = "LOST";
char  pkt_full[ 4 ] = "FULL";
char  pkt_quit[ 4 ] = "QUIT";


pluginclient_t *cursor_hook;
int             cursor_grabbed;

// Help texts
char  help[ 16 ][ 33 ] = {
  { "CONTROLS: WASD + MOUSE" },
  { "L: TOGGLE QWERTY, DVORAK, AZERTY" },
  { "H: SHOW/HIDE HELP" },
  { "F: TOGGLE FULL-SCREEN" },
  { "   WHEN IN WINDOWED MODE, USED" },
  { "   MOUSE-LEFT TO GRAB/UNGRAB" },
  { "ESCAPE: QUIT" },
  { "" },
};

unsigned int    message_timeout = 0;
unsigned char   help_count = 8;

