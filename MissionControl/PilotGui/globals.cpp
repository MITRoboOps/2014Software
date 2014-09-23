#include "globals.h"

linked_buf_t *p_buffer_last = new linked_buf_t;                   // Decoding buffer
linked_buf_t *p_buffer_first;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Surface* frame;
SDL_Texture* screen;
SDL_mutex *p_buffer_mutex = SDL_CreateMutex();
SDL_mutex *trust_mx = SDL_CreateMutex();
linked_buf_t  *trust_first = NULL;              // Non-lossy packet buffer
linked_buf_t  *trust_last = NULL;

UDPCli *vidCli;
UDPCli *ctrlCli;
OutputHandler *OutHdlr;
JoyData *joyData = new JoyData();

int cam[2];					//Which cameras are we viewing?
int vidTicks, ctrlTicks;

char  pkt_h264[4] = { 0x00, 0x00, 0x00, 0x01 };
char  pkt_data[4] = { 'D', 'A', 'T', 'A' };
char  pkt_ctrl[4] = { 'C', 'T', 'R', 'L' };

int trust_timeout;

bool thumbLock = false;