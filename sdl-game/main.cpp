#include <iostream>
#include <vector>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#undef main

#include "debug.h"
#include "global.h"
#include "gameObj.h"


int main(int argc, char *argv[])
{
    // init sdl
    if (!global::init(global::window, global::windowSurface))
    {
        DEBUG_MSG("Init failed");
        return -1;
    }

	// hide cursor
	SDL_ShowCursor(SDL_DISABLE);

	// containers
	// ==========

	// load textures
	std::map<std::string, SDL_Texture *> allTextures = {
		{"hello_world", global::loadTexture("hello_world.bmp")},
		{"arrows_up", global::loadTexture("arrows_up.bmp")},
	};

	// list of all objects
	std::vector<gameObj*> currentObjs = {
		new gameObj(allTextures["hello_world"], 10, 500, 10, 50, 50)
	};

	// player bullet container
	std::vector<gameObj*> currentPlayerBullets;

	// make player 
	// ===========

    // construct player
    gameObj *player = new gameObj(allTextures["arrows_up"], 10, global::SCREEN_WIDTH/2 - 10/2, global::SCREEN_HEIGHT/2 - 100/2, 100, 100);

	// set player bullet properties
	player->setBullet(allTextures["hello_world"], 25, 10, 10, 100);

    // game loop
    //===========
    while (!global::quit)
    {
        // event polling loop
        while (SDL_PollEvent(&global::event))
        {
            // window close event
            if (global::event.type == SDL_QUIT)
            {
                global::quit = true;
                break;
            }
        }
		
		// keybindings
		// ===========

		// quit
        if (global::keyState[SDL_SCANCODE_RETURN])
			global::quit = true;

		// toggle fullscreen F11
		if (global::keyState[SDL_SCANCODE_F11])
		{
			global::screenMode = global::screenMode == global::SCREEN_FULL ? global::SCREEN_WINDOWED : global::SCREEN_FULL;
			SDL_SetWindowFullscreen(global::window, global::screenMode);
		}

		// player keybindings
		// ==================
		// slow down
		if (global::keyState[SDL_SCANCODE_LSHIFT])
			player->setVelocityMod(.35);
		else
			player->setVelocityMod(1);

		// fire
		if (global::keyState[SDL_SCANCODE_Z] && SDL_TICKS_PASSED(SDL_GetTicks(), player->getBullet()->getTimeout()))
		{
			currentPlayerBullets.push_back(player->getNewBullet());
			player->getBullet()->resetTimeout();
		}

		// move left
		if (global::keyState[SDL_SCANCODE_LEFT] && player->getRectL() > 0)
			player->incRectX(-player->getVelocity() * player->getVelocityMod());

		// move right
		if (global::keyState[SDL_SCANCODE_RIGHT] && player->getRectR() < global::SCREEN_WIDTH)
			player->incRectX(player->getVelocity() * player->getVelocityMod());

		// move up
		if (global::keyState[SDL_SCANCODE_UP] && player->getRectTop() > 0)
			player->incRectY(-player->getVelocity() * player->getVelocityMod());

		// move down
		if (global::keyState[SDL_SCANCODE_DOWN] && player->getRectBottom() < global::SCREEN_HEIGHT)
			player->incRectY(player->getVelocity() * player->getVelocityMod());

        // render scene
        // ============

        // update window
        SDL_RenderClear(global::renderer);

        // render player
        SDL_RenderCopy(global::renderer, player->getCurrentTexture(), nullptr, &player->getRect());

        // render all current objs
        // =======================
		// TEMP ANIMATION
		for (int i = 0; i < currentObjs.size(); i++)
		{
			if (abs(currentObjs[i]->getInitialY() - currentObjs[i]->getRectY()) < 100)
				currentObjs[i]->incRectY(5);
			else
				currentObjs[i]->decRectX(5);

			if (currentObjs[i]->isOffscreen())
				currentObjs.erase(currentObjs.begin() + i);
			else
				SDL_RenderCopy(global::renderer, currentObjs[i]->getCurrentTexture(), nullptr, &currentObjs[i]->getRect());
		}
		
        // render all player bullets
        // =========================
		for(int i = 0; i < currentPlayerBullets.size(); i++)
		{
			currentPlayerBullets[i]->decRectY(currentPlayerBullets[i]->getVelocity());

			// remove bullet if offscreen
			if(currentPlayerBullets[i]->isOffscreen())
				currentPlayerBullets.erase(currentPlayerBullets.begin() + i);
			else
			//render bullet
				SDL_RenderCopy(global::renderer, currentPlayerBullets[i]->getCurrentTexture(), nullptr, &currentPlayerBullets[i]->getRect());
		}

        SDL_RenderPresent(global::renderer);

        SDL_Delay(16);
    }

    //==============
    // end game loop

    // close SDL subsystems
    global::close();

    return 0;
}
