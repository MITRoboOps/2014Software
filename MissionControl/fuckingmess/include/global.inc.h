#ifndef GLOBAL_INCLUDE
#define GLOBAL_INCLUDE
#include "../plugins/cli.h"



// Texts
extern char  text_contact[];
extern char  text_error[];
extern char  text_queued[];
extern char  text_full[];
extern char  text_lost[];
extern char  text_quit[];
extern char  text_version[];
extern char  text_blank[];
extern char  text_time[];
extern char  text_controls[];
extern char  text_timeout[];

// Packet types
extern char  pkt_h264[];
extern char  pkt_data[];
extern char  pkt_helo[];
extern char  pkt_time[];
extern char  pkt_ctrl[];
extern char  pkt_lost[];
extern char  pkt_full[];
extern char  pkt_quit[];


extern pluginclient_t *cursor_hook;
extern int             cursor_grabbed;

// Help texts
extern char  help[16][33];

extern unsigned int    message_timeout;
extern unsigned char   help_count;

#endif
