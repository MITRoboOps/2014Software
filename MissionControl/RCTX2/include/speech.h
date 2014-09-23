#ifndef _SAM_QUEUE_H_
#define _SAM_QUEUE_H_

void speech_open();
void speech_free();
void speech_poll();
void speech_queue( char* voice, char* speak );
int  speech_state();
int  speech_vis( char **buffer );

#endif