#include "include\SDL\SDL.h"
#include "include\SDL\SDL_image.h"
#include "include\SDL\SDL_ttf.h"
#include "SDLHelper.h"
#include "globals.h"


namespace SDLHelper {

	TTF_Font* fSml;
	TTF_Font* fMed;
	TTF_Font* fLrg;
	SDL_Color cWhite = { 255, 255, 255 };
	SDL_Color cBlack = { 0, 0, 0 };
	SDL_Color cGreen = { 154, 239, 160 };
	SDL_Color cRed	 = { 255, 138, 141 };

	bool SDLInit(){
		try
		{
			if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
				logSDLError("SDL_Init");
				return false;
			}

			window = SDL_CreateWindow("RoboOps: Pilot GUI", 100, 100, SCREEN_WIDTH,
				SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
			if (window == nullptr){
				logSDLError("CreateWindow");
				return false;
			}

			renderer = SDL_CreateRenderer(window, -1,
				SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (renderer == nullptr){
				logSDLError("CreateRenderer");
				return false;
			}
		
			//Load img extension (for png's etc.)
			if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
				logSDLError("IMG_Init");
				return false;
			}

			//Load true type font module
			if (TTF_Init() != 0){
				logSDLError("TTF_Init");
				return false;
			}
			atexit(TTF_Quit);
			//Load Fonts
			fSml = TTF_OpenFont("fonts/VeraMoBd.ttf", 12);
			fMed = TTF_OpenFont("fonts/VeraMoBd.ttf", 18);
			fLrg = TTF_OpenFont("fonts/VeraMoBd.ttf", 64);
			if (fSml == nullptr || fMed == nullptr || fLrg == nullptr){
				logSDLError("TTF_OpenFont");
				return false;
			}


		}
		catch (exception& e) 
		{
			Log::Write("SDLInit(): (SDL_GetError() failed with exception: " + (string)e.what() + ")", Log::ERR);
		}
		return true;
	}

	void logSDLError(const std::string &msg) {
		
			Log::Write("SDL Error: " + msg + " (" + SDL_GetError() + ")", Log::ERR);
		
	}

	SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren){
		SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
		if (texture == nullptr)
			logSDLError("LoadTexture");
		return texture;
	}

	/**
	* Draw an SDL_Texture to an SDL_Renderer at position x, y, with some desired
	* width and height
	* @param tex The source texture we want to draw
	* @param rend The renderer we want to draw too
	* @param x The x coordinate to draw too
	* @param y The y coordinate to draw too
	* @param w The width of the texture to draw
	* @param h The height of the texture to draw
	*/
	void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){
		//Setup the destination rectangle to be at the position we want
		SDL_Rect dst;
		dst.x = x;
		dst.y = y;
		dst.w = w;
		dst.h = h;
		SDL_RenderCopy(ren, tex, NULL, &dst);
	}

	/**
	* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
	* the texture's width and height
	* @param tex The source texture we want to draw
	* @param rend The renderer we want to draw too
	* @param x The x coordinate to draw too
	* @param y The y coordinate to draw too
	*/
	void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
		int w, h;
		SDL_QueryTexture(tex, NULL, NULL, &w, &h);
		renderTexture(tex, ren, x, y, w, h);
	}

	/**
	* Render the message we want to display to a texture for drawing
	* @param message The message we want to display
	* @param color The color we want the text to be
	* @param renderer The renderer to load the texture in
	* @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
	*/
	SDL_Surface* renderText(const std::string &message, SDL_Color color, TTF_Font* font)
	{
		
		//We need to first render to a surface as that's what TTF_RenderText
		//returns, then load that surface into a texture
		SDL_Surface *surf = TTF_RenderText_Blended_Wrapped(font, message.c_str(), color,1000);
		if (surf == nullptr){
			logSDLError("TTF_RenderText");
			return nullptr;
		}
		return surf;
	}


}
