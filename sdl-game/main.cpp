#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#undef main

#define DEBUG

#include "debug_msg.h"
#include "global.h"
#include "classes.h"


int main(int argc, char *argv[])
{

	// player
	playerObj player(1, 1, 100);
	std::cout << player.rect.w << std::endl;

	// init sdl
	if (!g::init(g::window, g::windowSurface))
	{
		DEBUG_MSG("Init failed");
		return -1;
	}


	// list of all objects with collidable rectangles
	std::vector<SDL_Rect> collidableObj;

	// game loop
	//===========
	while (!g::quit)
	{
		// event polling loop
		while (SDL_PollEvent(&g::event))
		{
			// window close event
			if (g::event.type == SDL_QUIT)
			{
				g::quit = true;
				break;
			}
		}

		// check keystate
		//===============
		#ifdef DEBUG
		if (g::keyState[SDL_SCANCODE_RETURN]) // DEBUG: quick quitting
			g::quit = true;
		#endif
		
		// update window
		SDL_RenderClear(g::renderer);
		SDL_RenderCopy(g::renderer, player.getCurrentTexture(), nullptr, &player.rect);
		SDL_RenderPresent(g::renderer);

		SDL_Delay(16);
	}

	//==============
	// end game loop

	// close SDL subsystems
	g::close(g::windowSurface, g::window);

	return 0;
}