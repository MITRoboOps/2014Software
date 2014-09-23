#pragma once
#include <sstream>
#include "include\SDL\SDL.h"
#include "include\SDL\SDL_ttf.h"


namespace SDLHelper {

	

	extern TTF_Font* fSml;
	extern TTF_Font* fMed;
	extern TTF_Font* fLrg;
	extern SDL_Color cWhite;
	extern SDL_Color cBlack;
	extern SDL_Color cRed;
	extern SDL_Color cGreen;

	bool SDLInit();

	void logSDLError(const std::string &msg);

	SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);

	void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);
	void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);

	SDL_Surface* renderText(const std::string &message, SDL_Color color, TTF_Font* font);

};

