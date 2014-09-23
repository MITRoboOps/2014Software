#include <stdio.h>
#include <iostream>

#include "include\SDL\SDL.h"
#include "include\SDL\SDL_image.h"
#include "globals.h"
#include "InputHandler.h"
#include "OutputHandler.h"
#include "MSGParser.h"

#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
extern "C" {
	#include "include\libswscale\swscale.h"
	#include "include\libavcodec\avcodec.h"
	#include "include\libavformat\avformat.h"
}

#include "logger.h"
#include "SDLHelper.h"
#include "UDPCli.h"



#define CLIENT_RPS 30
#define BIG_TICK 30
#define TIMEOUT_TRUST         16 // Before retransmitting trusted packets


using namespace std;
using namespace SDLHelper;


int main(int argc, char *argv[]) {

	//LOCAL VARS
	int					temp;                       // Various uses
	//--- VIDEO DECODING
	AVCodecContext		*pCodecCtx;
	AVCodec				*pCodec;
	AVFrame				*pFrame;
	AVPacket			avpkt;
	struct SwsContext	*convertCtx;            // Used in the scaling/conversion process
	SDL_Surface			*live = NULL;           // Live decoded video surface

	//--- HUD STUFF
	stringstream T;
	char cTemp[512];
	T << "Hello World";
	struct textTexture{
		SDL_Texture* Tex;
		int x, y;
	};
	textTexture *textJoyDat = new textTexture();
	textTexture *textVidCon = new textTexture();
	textTexture *textCtrlCon = new textTexture(); 
	textTexture *textFPS = new textTexture();
	textTexture *textStatus = new textTexture();
	//SDL_Texture* joyDat = nullptr;
	//SDL_Texture* joyDat = nullptr;
	SDL_Surface* sText = nullptr;
//	const uint8_t * data[1];
	//int linesize[1];


	//--- PERFORMANCE MONITORING
	Uint32             time_target;             // Timing target
	Sint32             time_diff;               // Timing differential
	int				   ticks = 0;
	int				   fpscounter;

	//--- NETWORKING
	char               p_ctrl[8192];            // CTRL packet buffer
	
	

	//
	// INITIALIZATION
	//
	Log::Write("MIT Robotics Team: Pilot GUI V0.1.\nLoading...", Log::INFO);

	//Initialize Global Vars
	trust_timeout = 0;
	cam[0] = 9;
	cam[1] = 9;
	ctrlTicks = 0;
	vidTicks = 0;
	fpscounter = SDL_GetTicks();
	
	//Allocate stuff
	p_buffer_first = p_buffer_last;
	p_buffer_last->size = 0;

	//NETWORK
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		Log::Write("UDP Network initialization failed.", Log::ERR);
		return false;
	}
	vidCli = new UDPCli("192.168.0.10", 6979);
	ctrlCli = new UDPCli("192.168.0.10", 9930);
	Log::Write("Network OK! ", Log::DEBUG);

	//OUTPUT HANDLER
	OutHdlr = new OutputHandler();

	//SDL
	if (!SDLInit())
		return 2;
	//SDL_Texture *image = loadTexture("2.png", renderer);
	//if (image == nullptr)
	//	return 4;

	//Joystick
	InputHandler* inHandler = new InputHandler();
	if(!inHandler->OpenAll())
		logSDLError("OpenJoystick Failed.");
	Log::Write("Joysticks OK! ", Log::DEBUG);

	//FFMPEG
	avcodec_register_all();
	pCodecCtx = avcodec_alloc_context3(NULL);
	pCodec = avcodec_find_decoder(CODEC_ID_H264);
	av_init_packet(&avpkt);
	if (!pCodec) {
		printf("ERROR: Unable to initialize decoder\n");
	}
	avcodec_open2(pCodecCtx, pCodec, NULL);
	pFrame = av_frame_alloc();
	
	frame = SDL_CreateRGBSurface(SDL_SWSURFACE, SCREEN_WIDTH, SCREEN_HEIGHT, 24, 0, 0, 0, 0);
	if (!frame)
		logSDLError("Frame Alloc Failed.");
	
	Log::Write("FFMPEG OK! ", Log::DEBUG);
	 

	//Where are we putting our image
	int iW, iH;
	//SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
	//int x = SCREEN_WIDTH / 2 - iW / 2;
	//int y = SCREEN_HEIGHT / 2 - iH / 2;

	//textJoyDat->Tex = SDL_CreateTextureFromSurface(renderer, sText);

	SDL_RenderClear(renderer);
	
	SDL_Event e;
	bool quit = false;

	//~---TESTCODE
	//ctrl_data_t* ctrl = new ctrl_data_t();
	time_target = SDL_GetTicks();

	//****************************
	//****************************
	// MAIN LOOP *****************
	//****************************
	//****************************
	Log::Write("\nLoading Complete!\n", Log::DEBUG);
	vidCli->uSend("HELO",4);
	while (!quit)
	{
		ticks++;

		//****************************
		// HANDLE INPUT **************
		//****************************
		while (SDL_PollEvent(&e)){
			if (e.type == SDL_QUIT)
				quit = true;
			else if (e.type == SDL_KEYDOWN)
			{
				switch ((char)e.key.keysym.sym)
				{
					case 'r': // Reconnect
						vidCli->Disconnect(); 
						ctrlCli->Disconnect();
						WSACleanup();
						WSADATA wsaData;
						if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
							Log::Write("UDP Network initialization failed.", Log::ERR);
							return false;
						}
						vidCli->Connect();
						ctrlCli->Connect();
						vidCli->uSend("HELO", 4);
						printf("Reconnected!\n");
						break;
					case 's': // Stow
						ctrlCli->uSend("T120A0", 6);
						break;
					case 'a': // rdy pos
						ctrlCli->uSend("T122A1", 6);
						break;
					case 'd': // Deposit Rock
						ctrlCli->uSend("T123A2", 6);
						break;
					case 'c': // Reset Counter
						ctrlCli->uSend("T124A3", 6);
						break;
					case 'x': // Abort
						ctrlCli->uSend("T125A4",6);
						break;
					case 'l': // Down
						ctrlCli->uSend("T120B0", 6);
						break;
					case 'o': // Up
						ctrlCli->uSend("T120B1", 6);
						break;
					case 'p': // Trim Up
						ctrlCli->uSend("T120B2", 6);
						break;
					case ';': // Trim Down
						ctrlCli->uSend("T120B3", 6);
						break;
					case 27:
						quit = true;
						break;
				}
			}
			else  if (e.type == SDL_MOUSEBUTTONDOWN)
				quit = false;
			else if (e.type == SDL_JOYHATMOTION)
				inHandler->getHats(&e);
			else if (e.type == SDL_JOYAXISMOTION)
				inHandler->getAxes(&e);
			else if (e.type == SDL_JOYBUTTONDOWN)
				inHandler->HandleBtn(&e);
		}
		//Render the scene
		SDL_RenderClear(renderer);
		//printf("a\n");
		//****************************
		// HANDLE OUTPUT *************
		//****************************
		OutHdlr->sendChannels();

		//****************************
		// HANDLE NETWORK IO *********
		//****************************
		
		//Video
		memcpy(&p_ctrl, pkt_ctrl, 4);
		p_ctrl[4] = cam[0];
		p_ctrl[5] = cam[1];
		vidCli->uSend(p_ctrl, 6);


		//****************************
		// UPDATE HUD INFO ***********
		//****************************

		T.str("");
		T << "Input Type: " << inHandler->joyMode
			<< ", 0=" << joyData->axis[0]
			<< ", 1=" << joyData->axis[1]
			<< ", 2=" << joyData->axis[2]
			<< ", 3=" << joyData->axis[3]
			<< ", 4=" << joyData->axis[4]
			<< ", 5=" << joyData->axis[5]
			<< ", 6=" << joyData->axis[6]
			<< ", 7=" << joyData->axis[7]
			<< ", 8=" << joyData->axis[8]
			<< ", 9=" << joyData->axis[9]
			<< ", 10=" << joyData->axis[10]
			<< ", 11=" << joyData->axis[11];

		SDL_DestroyTexture(textJoyDat->Tex);
		sText = renderText(T.str(), cWhite, fSml);
		textJoyDat->Tex = SDL_CreateTextureFromSurface(renderer, sText);
		SDL_FreeSurface(sText);
		SDL_QueryTexture(textJoyDat->Tex, NULL, NULL, &iW, &iH);
		textJoyDat->x = SCREEN_WIDTH / 2 - iW / 2;
		textJoyDat->y = SCREEN_HEIGHT - 20;
		
		if (MSGParser::update)
		{
			MSGParser::update = false;
			T.str("");

			sprintf_s(cTemp, 512, "UPTIME: %s  BATT: %2.2f[V]  LOAD: %2.2f[A]  CHGUSED:%05d[mAh]  PITCH: %3.1f[deg]  ROLL: %3.1f[deg]",
					MSGParser::status.uptime,
					MSGParser::status.vBatt,
					MSGParser::status.currNetDraw,
					MSGParser::status.currConsumed,
					MSGParser::status.pitch,
					MSGParser::status.roll);
			T << cTemp;
			SDL_DestroyTexture(textStatus->Tex);
			sText = renderText(T.str(), cWhite, fSml);
			textStatus->Tex = SDL_CreateTextureFromSurface(renderer, sText);
			SDL_FreeSurface(sText);
			//SDL_QueryTexture(textStatus->Tex, NULL, NULL, &iW, &iH);
			textStatus->x = 5;
			textStatus->y = 5;
		}

		//****************************
		// UPDATE VIDEO STREAM *******
		//****************************
		if (p_buffer_first->size) {
			//do_decode = 1;
			
			// Decode frame
			while (p_buffer_first != p_buffer_last)
			{
				if (SDL_LockMutex(p_buffer_mutex) == 0)
				{
					avpkt.data = (unsigned char*)p_buffer_first->data;
					avpkt.size = p_buffer_first->size;
					avpkt.flags = AV_PKT_FLAG_KEY;

					SDL_UnlockMutex(p_buffer_mutex);
				}


				if (avcodec_decode_video2(pCodecCtx, pFrame, &temp, &avpkt) < 0)
					Log::Write("NOTICE: Decoding error (packet loss)\n", Log::WARN);
				else
				{
					vidTicks = 5;


					SDL_LockSurface(frame);

					const uint8_t * data[1] = { (uint8_t*)frame->pixels };
					int linesize[1] = { frame->pitch };

					// Create scaling & color-space conversion context
					convertCtx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
						frame->w, frame->h, PIX_FMT_RGB24, SWS_AREA, NULL, NULL, NULL);
					// Scale and convert the frame
					sws_scale(convertCtx, (const uint8_t**)pFrame->data, pFrame->linesize, 0,
						pCodecCtx->height, (uint8_t * const*)data, linesize);

					// Cleanup
					sws_freeContext(convertCtx);

					SDL_UnlockSurface(frame);


					// Convert to display format for fast blitting
					if (live)
						SDL_FreeSurface(live);

					live = SDL_ConvertSurfaceFormat(frame, SDL_PIXELFORMAT_RGB24, 0);

				}
				// Pop decoding buffer from queue
				if (SDL_LockMutex(p_buffer_mutex) == 0)
				{

					linked_buf_t *p_buf = p_buffer_first;
					p_buffer_first = p_buffer_first->next;
					delete[] p_buf;
				}
				SDL_UnlockMutex(p_buffer_mutex);
			}
			

		}

		if (live) {
			// Draw video
			SDL_DestroyTexture(screen);
			//SDL_UpdateTexture(screen, frame->pixels(),
			screen = SDL_CreateTextureFromSurface(renderer,live);
			//SDL_BlitSurface(live, NULL, screen, NULL);
		}
		else {
			// Clear screen
			//SDL_FillRect(screen, rect(&r, 0, 0, screen->w, screen->h), 0);
		}


		

		//****************************
		// BIGTICK **************
		//****************************
		if (ticks >= BIG_TICK)
		{
			ticks = 0;

			//FPS Counter
			fpscounter = SDL_GetTicks() - fpscounter;
			if (fpscounter>0)
			fpscounter = (int)(BIG_TICK/(fpscounter/1000.0));
			T.str("");
			T << "FPS: " << fpscounter;
			SDL_DestroyTexture(textFPS->Tex);
			sText = renderText(T.str(), cWhite, fSml);
			textFPS->Tex = SDL_CreateTextureFromSurface(renderer, sText);
			SDL_FreeSurface(sText);
			//SDL_QueryTexture(textJoyDat, NULL, NULL, &iW, &iH);
			textFPS->x = SCREEN_WIDTH -80;
			textFPS->y = 20;
			fpscounter = SDL_GetTicks();

			//UDP Video Connection Status
			T.str("");
			T << "Video: ";
			if (vidTicks>0)
				sText = renderText(T.str()+"Connected", cGreen, fMed);
			else
				sText = renderText(T.str() + "Disconnected", cRed, fMed);
			SDL_DestroyTexture(textVidCon->Tex);
			textVidCon->Tex = SDL_CreateTextureFromSurface(renderer, sText);
			SDL_FreeSurface(sText);
			SDL_QueryTexture(textVidCon->Tex, NULL, NULL, &iW, &iH);
			textVidCon->x = SCREEN_WIDTH / 2 - iW - 10;
			textVidCon->y = 20;
			vidTicks--;

			//UDP Control Connection Status
			T.str("");
			T << "Control: ";
			if (ctrlTicks>0)
				sText = renderText(T.str() + "Connected", cGreen, fMed);
			else
				sText = renderText(T.str() + "Disconnected", cRed, fMed);
			SDL_DestroyTexture(textCtrlCon->Tex);
			textCtrlCon->Tex = SDL_CreateTextureFromSurface(renderer, sText);
			SDL_FreeSurface(sText);
			SDL_QueryTexture(textCtrlCon->Tex, NULL, NULL, &iW, &iH);
			textCtrlCon->x = (SCREEN_WIDTH / 2) + 10;
			textCtrlCon->y = 20;
			ctrlTicks--;

			//STATUS
			
		}

		//****************************
		// FINAL RENDER **************
		//****************************

		renderTexture(screen, renderer, 0, 0);
		//renderTexture(image, renderer, x, y);
		renderTexture(textJoyDat->Tex, renderer, textJoyDat->x, textJoyDat->y);
		renderTexture(textFPS->Tex, renderer, textFPS->x, textFPS->y);
		renderTexture(textVidCon->Tex, renderer, textVidCon->x, textVidCon->y);
		renderTexture(textCtrlCon->Tex, renderer, textCtrlCon->x, textCtrlCon->y);
		renderTexture(textStatus->Tex, renderer, textStatus->x, textStatus->y);
		SDL_RenderPresent(renderer);

		//****************************
		// HOUSEKEEPING **************
		//****************************

		// Delay 1/CLIENT_RPS seconds, constantly correct for processing overhead
		time_diff = SDL_GetTicks() - time_target;
		//printf( "decode: %i, time: %i, diff: %i, ", do_decode, SDL_GetTicks(), time_diff );
		if (time_diff > 1000) {
			printf("WARN: Cannot keep up with the desired RPS\n");
			time_target = SDL_GetTicks();
			time_diff = (1000 / CLIENT_RPS);
		}
		time_target += 1000 / CLIENT_RPS;
		//printf( "target: %i, delay: %i\n", time_target, MAX( 0, ( 1000 / CLIENT_RPS ) - time_diff ) );
		SDL_Delay(max(0, (1000 / CLIENT_RPS) - time_diff));
	}


	//SDL_DestroyTexture(background);
	//SDL_DestroyTexture(image);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	
	IMG_Quit();
	SDL_Quit();
	return 0;
}
