#include "global.h"
#include "debug.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace g {

bool quit = false;

const Uint8 SCREEN_WINDOWED = 0;
const Uint8 SCREEN_FULL = SDL_WINDOW_FULLSCREEN;
Uint8 screenMode = SCREEN_WINDOWED;

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

// left, right, top, bottom
SDL_Rect screenEdge[SCREEN_EDGE_TOTAL] = {
	makeRect(0, 0, 1, SCREEN_HEIGHT),
	makeRect(SCREEN_WIDTH - 1, 0, 1, SCREEN_HEIGHT),
	makeRect(0, 0, SCREEN_WIDTH, 1),
	makeRect(0, SCREEN_HEIGHT - 1, SCREEN_WIDTH, 1)
};

SDL_Window *window = nullptr; // main window
SDL_Surface *windowSurface = nullptr; // surface for main window
SDL_Renderer *renderer = nullptr; // main renderer

// realtime keystate
const Uint8 *keyState = SDL_GetKeyboardState(nullptr);

// event handler
SDL_Event event;

// functions
// =========

// SDL rect wrapper
SDL_Rect makeRect(const int &xPos, const int &yPos, const int &width, const int &height)
{
	SDL_Rect rect;
	rect.x = xPos;

	rect.y = yPos;
	rect.w = width;
	rect.h = height == -1 ? width : height;

	return rect;
}

bool init(SDL_Window *&window, SDL_Surface *&windowSurface)
{
	DEBUG_MSG("** Begin init **");

	// init video
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not init video: " << SDL_GetError() << std::endl;
		return false;
	}
	DEBUG_MSG("Init video");

	// init PNG loading
	if (!IMG_Init(IMG_INIT_PNG))
	{
		std::cout << "Could not init PNG loading: " << SDL_GetError() << std::endl;
		return false;
	}
	DEBUG_MSG("Init PNG loading");

	// create window
	window = SDL_CreateWindow("SDL Tutorial",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);

	if (window == nullptr)
	{
		std::cout << "SDL window creation error: " << SDL_GetError() << std::endl;
		return false;
	}
	DEBUG_MSG("Created window");

	// init renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == nullptr)
	{
		std::cout << "Could not init renderer: " << SDL_GetError() << std::endl;
		return false;
	}
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
	DEBUG_MSG("Init renderer");

	// assign window surface
	windowSurface = SDL_GetWindowSurface(window);

	DEBUG_MSG("** End init **");

	return true;
}

SDL_Surface *loadImage(const char fileName[])
{
	// load image
	SDL_Surface *imageSurface = IMG_Load(fileName);
	SDL_Surface *optimizedSurface = nullptr;

	if (imageSurface == nullptr)
	{
		std::cout << "Unable to load image " << fileName << ": " << SDL_GetError() << std::endl;
		return nullptr;
	}

	// optimize image
	optimizedSurface = SDL_ConvertSurface(imageSurface, windowSurface->format, 0);
	if (optimizedSurface == nullptr)
	{
		std::cout << "Unable to optimize surface " << fileName << ": " << std::endl;
		return nullptr;
	}

	// free unoptimized surface
	SDL_FreeSurface(imageSurface);

	DEBUG_MSG("Load image successful: " << fileName);

	return optimizedSurface;
}

// IMG_LoadTexture wrapper
SDL_Texture *loadTexture(const char fileName[])
{
	SDL_Texture *texture = IMG_LoadTexture(renderer, fileName);

	if (texture == nullptr)
	{
		std::cout << "Unable to load texture: " << fileName << " : " << SDL_GetError() << std::endl;
		return nullptr;
	}

	DEBUG_MSG("Load texture successful: " << fileName);

	return texture;
}

bool close()
{
	//Deallocate windowSurface
	SDL_FreeSurface(windowSurface);
	windowSurface = nullptr;

	//Destroy window
	SDL_DestroyWindow(window);
	window = nullptr;

	// Destroy renderer
	SDL_DestroyRenderer(renderer);

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();

	DEBUG_MSG("Close successful");
	return true;
}

} // end namespace