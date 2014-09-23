#pragma once
#include "logger.h"
#include "include\SDL\SDL.h"
#include "UDPCli.h"
#include "OutputHandler.h"
#include <stdint.h>

#ifdef __GNUC__
#define PACKED( class_to_pack ) class_to_pack __attribute__((__packed__))
#else
#define PACKED( class_to_pack ) __pragma( pack(push, 1) ) class_to_pack __pragma( pack(pop) )
#endif

const int SCREEN_WIDTH = 2200;
const int SCREEN_HEIGHT = 1225/2;

extern int cam[2];					//Which cameras are we viewing?
extern int vidTicks, ctrlTicks;

struct linked_buf_t {
	uint8_t data[8192];
	int32_t size;
	linked_buf_t *next;
};

// DISP packet
typedef struct {
	uint8_t trust_srv;
	uint8_t trust_cli;
	uint16_t pad;
	int32_t timer;
} disp_data_t;

PACKED(
	typedef struct {
		int32_t mx;
		int32_t my;
		int32_t dx;
		int32_t dy;
		uint8_t kb; // kb_bitmask_e
		uint8_t pad[3];
	} ctrl_t;
)

struct JoyData {
	int axis[12]; // Axes: 0 - X1,  1 - Y1,  2 - R1, 3- Trim1, 4 - X2, 5 - Y2, 6 - R2 7 - Trim2 
				  //	   8 - Hat1X, 9 - Hat1Y, 10 - Hat2X, 11 - Hat2Y
};
extern JoyData* joyData;

// DATA packet
PACKED(
	typedef struct {
		uint8_t trust_srv;
		uint8_t trust_cli;
		uint8_t pad[2];
		ctrl_t ctrl;
	} ctrl_data_t;
)
extern UDPCli *vidCli;
extern UDPCli *ctrlCli;
extern OutputHandler *OutHdlr;

///

extern linked_buf_t *p_buffer_last;                   // Decoding buffer
extern linked_buf_t *p_buffer_first;
extern linked_buf_t  *trust_first;              // Non-lossy packet buffer
extern linked_buf_t  *trust_last;

typedef struct linked_buf_t linked_buf_t;

extern SDL_mutex *p_buffer_mutex;
extern SDL_mutex *trust_mx;
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Surface* frame;
extern SDL_Texture* screen;

extern int trust_timeout;

// Packet types
extern char  pkt_h264[4];
extern char  pkt_data[4];
extern char  pkt_ctrl[4];
/*char  pkt_helo[4] = "HELO";
char  pkt_time[4] = "TIME";
char  pkt_ctrl[4] = "CTRL";
char  pkt_lost[4] = "LOST";
char  pkt_full[4] = "FULL";
char  pkt_quit[4] = "QUIT";
char  pkt_rosm[4] = "ROSM";*/

extern bool thumbLock;
