#include <math.h>
#include <SDL/SDL.h>
#include "robocortex.h"
#ifdef USE_SAM
#include "include/sam/sam.h"
#else
#include "include/espeak/speak_lib.h"
#endif 

struct speech_list_t {
	char voice[ 256 ];
	char data[ 256 ];
	struct speech_list_t *next;
};

typedef struct speech_list_t speech_list_t;

// SAM linked-list and mutex
static speech_list_t *speech_first = NULL;
static speech_list_t *speech_last = NULL;
static  SDL_mutex    *speech_mx;

// SPEECH buffers
static uint8_t        buf[ 2 * 10 * 44100 ];
#ifndef USE_SAM
static uint8_t       *p_buf;
#endif 
static int            buf_size;
static int            buf_pos;
static int            speaking = 0;

// Visualization
static unsigned char vis_buffer[ 160 ];
static unsigned char vis_mul[ 160 ];
static int  do_vis = 0;
static int ctr = 0;

#define MAX_VISEME 1000

static uint16_t viseme_count;
static uint8_t viseme_last = 0;
static viseme_t viseme_list[ MAX_VISEME ];

static int freq;
static int opened;

int speech_vis( unsigned char **buffer ) {
	do_vis = 1;
	if( speaking == 1 ) {
		*buffer = vis_buffer;
		return( 0 );
	}
	return( -1 );
}

static void sdl_mixer( void *unused, Uint8 *stream, int stream_len ) {
	int i;
	float f,dc;
	int len=stream_len;
	static uint8_t offset = 0;
	uint8_t viseme = 0;
	if( buf_pos >= buf_size ) {
    if( speaking == 1 ) speaking = 2;
    if( viseme_last != 0 ) {
    	viseme_last = 0;
    	speak_viseme( 0 );
    }
	} else {
#ifdef USE_SAM
		if( ( buf_size - buf_pos ) < len ) len = buf_size - buf_pos;
		for( i = 0; i < len; i++ ) {
			stream[ i ] = buf[ buf_pos++ ];
		  buf_pos++;
		}
		if( len < stream_len ) memset( &stream[ len ], 128, stream_len - len );

		if( do_vis ) {
			f = stream[ 0 ];
			dc = f;
			vis_buffer[ 0 ] = ( ( ( int )( f - dc ) ) * vis_mul[ i ] ) >> 8;
		  if( len > 160 ) len = 160;
			for( i = 1; i < len; i++ ) {
				f += ( ( ( float )( char )stream[ ( i << 1 )  ] ) - f ) / 10;
				dc += ( f - dc ) / 20;
				vis_buffer[ i ] = ( ( ( int )( f - dc ) ) * vis_mul[ i ] ) >> 8;
			}
			for( ; i < 160; i++ ) {
				f += ( ( ( float )0 ) - f ) / 10;
				dc += ( f - dc ) / 20;
				vis_buffer[ i ] = ( ( ( int )( f - dc ) ) * vis_mul[ i ] ) >> 8;
			}
		}
#else		
		for( i = 0; i < viseme_count; i++ ) {
			if( viseme_list[ i ].time > buf_pos ) break;
			viseme = viseme_list[ i ].viseme;
		}
		if( viseme != viseme_last ) {
			viseme_last = viseme;
			speak_viseme( viseme );
		}
		if( ( buf_size - buf_pos ) < len ) len = buf_size - buf_pos;
		memcpy( stream, &buf[ buf_pos ], len );
		buf_pos += len;
		if( len < stream_len ) memset( &stream[ len ], 0, stream_len - len );

		if( do_vis ) {
		  offset = ( offset + 28 ) % 512; // Synchronizes waveform for +F2 frequency (not necessary, just looks cool)
		  if( len > 160 ) len = 160;
			for( i = 1; i < len; i++ ) {
				f = ( ( ( float )*( int* )&stream[ ( i << 2 ) + offset ] ) );
				vis_buffer[ i ] = ( ( ( int )( f / 30000000.0 ) ) * vis_mul[ i ] ) >> 8;
			}
			for( ; i < 160; i++ ) vis_buffer[ i ] = 0;
		}
		
#endif
		

	}

}

#ifndef USE_SAM

int viseme2mouth( int viseme ) {
	const uint8_t mouth_table[ 22 ] = {
		0,  // Silence         
	  11, // AE, AX, AH    
	  11, // AA            
	  11, // AO            
	  10, // EY, EH, UH    
	  11, // ER            
	  9,  // y, IY, IH, IX 
	  2,  // w, UW         
	  13, // OW            
	  9,  // AW            
	  12, // OY            
	  11, // AY            
	  9,  // h             
	  3,  // r             
	  6,  // l             
	  7,  // s, z          
	  8,  // SH, CH, JH, ZH
	  5,  // TH, DH        
	  4,  // f, v          
	  7,  // d, t, n       
	  9,  // k, g, NG      
	  1	  // p, b, m         
	};
	if( viseme > 21 ) viseme = 0;
	return( mouth_table[ viseme ] );
}

// Convert eSpeak phoneme name into a SAPI viseme code
int phoneme2viseme( unsigned int phoneme_name ) {
	int ix;
	unsigned int ph;
	unsigned int ph_name;

#define PH( c1, c2 ) ( c2 << 8 ) + c1          // combine two characters into an integer for phoneme name 

	const unsigned char initial_to_viseme[128] = {
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,
		 0,  0,  0,  0,  0,  0,  1,  0,  0,  0, 19,  0,  0,  0,  0,   0,
		 0,  0,  0,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 255,
		 4,  2, 18, 16, 17,  4, 18, 20, 12,  6, 16, 20, 14, 21, 20,   3,
		21, 20, 13, 16, 17,  4,  1,  5, 20,  7, 16,  0,  0,  0,  0,   0,
		 0,  1, 21, 16, 19,  4, 18, 20, 12,  6,  6, 20, 14, 21, 19,   8,
		21, 20, 13, 15, 19,  7, 18,  7, 20,  7, 15,  0,  0,  0,  0,   0 };

	const unsigned int viseme_exceptions[] = {
		PH( 'a' , 'I' ), 11,
		PH( 'a' , 'U' ),  9,
		PH( 'O' , 'I' ), 10,
		PH( 't' , 'S' ), 16,
		PH( 'd' , 'Z' ), 16,
		PH( '_' , '|' ), 255,
		0
	};
	
	ph_name = phoneme_name & 0xffff;
	for( ix=0; ( ph = viseme_exceptions[ix] ) != 0; ix += 2 ) {
		if( ph == ph_name ) return( viseme_exceptions[ ix + 1 ] );
	}
	return( initial_to_viseme[ phoneme_name & 0x7f ] );
}

int espeak_cb( short *pcm_s16, int samples, espeak_EVENT *events ) {
	espeak_EVENT *e;

	for( e = events; e->type != 0; e++ ) {
		if( e->type == espeakEVENT_PHONEME ) {
			if( viseme_count < MAX_VISEME ) {
				viseme_list[ viseme_count ].viseme = viseme2mouth( phoneme2viseme( e->id.number ) );
				viseme_list[ viseme_count ].time = ( e->audio_position * freq ) / 500;
				viseme_count++;
			}
			//printf( "Viseme: %i @ %i\n", VisemeCode( e->id.number ), ( e->audio_position * freq ) / 1000 );
		}
	}
	
	
  if( samples > 0 ) {
  	printf( "Samples: %i\n", samples );
    if( p_buf == buf ) buf_size = 0;
    buf_pos = 0;
    samples *= sizeof( short );
    if( samples > &buf[ sizeof( buf ) ] - p_buf ) samples = &buf[ sizeof( buf ) ] - p_buf;
    memcpy( p_buf, pcm_s16, samples );
    p_buf += samples;
    buf_size += samples;
  }
  return( 0 );
}
#endif

void speech_open() {
  int n;
  float m;
  speech_mx = SDL_CreateMutex();
  SDL_AudioSpec fmt;

#ifdef USE_SAM
  freq = 11025;
#else
  freq = espeak_Initialize( AUDIO_OUTPUT_SYNCHRONOUS, 10000, ".", 1 );
  printf( "Speech [info]: eSpeak frequency: %i\n", freq );
  espeak_SetSynthCallback( espeak_cb );
#endif 

  fmt.freq     = freq;
#ifdef USE_SAM
  fmt.format   = AUDIO_U8;
#else
  fmt.format   = AUDIO_S16;
#endif   
  fmt.channels = 1;
  fmt.samples  = 1024;
  fmt.callback = sdl_mixer;
  fmt.userdata = NULL;
  for( n = 0; n < 160; n++ ) {
    vis_mul[ n ] = ( unsigned char )( ( 1.0 - cos( ( ( float )n ) / 80.0 * 3.1415f ) ) * 127 );
  }
  if ( SDL_OpenAudio( &fmt, NULL ) < 0 ) {
    fprintf( stderr, "Speech [error]: Unable to open SDL audio\n" );
  } else {
    printf( "Speech [info]: Initialized\n" );
    opened = 1;
  }

}

static void buf_play() {
#ifdef USE_SAM	
	buf_size    /= SAM_SCALE;
	buf_size = ( buf_size > 512 ? buf_size - 512 : 0 );
#endif
  buf_pos      = 0;
  speaking = 1;
	SDL_PauseAudio( 0 );
}

uint8_t speech_poll() {
  speech_list_t *p_list;
	if( speaking == 2 ) {
    SDL_PauseAudio( 1 );
    speaking = 0;
	}
	if( speaking == 0 ) {
    SDL_mutexP( speech_mx );
		if( speech_first ) {
  		// Queue pop
      p_list = speech_first;
      speech_first = p_list->next;
      SDL_mutexV( speech_mx );
#ifdef USE_SAM
  		// Set parameters
  		sam_params( SAM_SCALE, SAM_SING, SAM_SPEED, SAM_PITCH, SAM_MOUTH, SAM_THROAT );
      buf_pos = 0;
  		buf_size = sizeof( buf );
  		if( sam_speak( buf, &buf_size, p_list->data ) == 0 ) buf_play();
#else

      printf( "Speech [info]: eSpeak voice return: %i\n", espeak_SetVoiceByName( p_list->voice ) );
      espeak_SetParameter(espeakRATE,130,0);
	    espeak_SetParameter(espeakRANGE,0,0);
//		espeak_SetParameter(espeakVOLUME,volume,0);
//		espeak_SetParameter(espeakPITCH,pitch,0);
//		espeak_SetParameter(espeakCAPITALS,option_capitals,0);
//		espeak_SetParameter(espeakPUNCTUATION,option_punctuation,0);
//		espeak_SetParameter(espeakWORDGAP,wordgap,0);
//		espeak_SetParameter(espeakLINELENGTH,option_linelength,0);
//		espeak_SetPunctuationList(option_punctlist);

      p_buf = buf;
      viseme_count = 0;
      espeak_Synth( p_list->data, strlen( p_list->data ) + 1, 0, POS_CHARACTER, 0, 0, NULL, NULL );
      buf_play();
#endif
  		free( p_list );
		} else {
      SDL_mutexV( speech_mx );
		}
	}
	return( speaking );
}

void speech_queue( char* voice, char* speak ) {
  speech_list_t *p_list = malloc( sizeof( speech_list_t ) );

  if( !opened ) return;
    
  if( p_list == NULL ) {
    printf( "Speech [error]: Out of memory" );
    return;
  }

  strcpy( p_list->voice, voice );
#ifdef USE_SAM
  // Copy and convert to phenomes
  memset( p_list->data, 0, 256 );
  strcpy( p_list->data, speak );
  sam_phenomes( p_list->data );
  strcat( p_list->data, " \x9b\0" ); // TODO: is this really necessary?
#else
  strcpy( p_list->data, speak );
#endif

 	// Queue push
  SDL_mutexP( speech_mx );
	p_list->next = NULL;
	if( speech_first ) {
		speech_last->next = p_list;
	} else {
		speech_first = p_list;
	}  
	speech_last = p_list;
  SDL_mutexV( speech_mx );
	
	speech_poll();

}

void speech_free() {
  SDL_DestroyMutex( speech_mx );
}

int speech_state() {
	return( speaking );
}
