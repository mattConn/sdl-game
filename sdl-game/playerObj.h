#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "global.h"
#include "gameObj.h"

// player object
// =============
class playerObj : public gameObj {

	// keypress enum for relating textures to keypress events
	enum KeyPresses
	{
		KEY_PRESS_DEFAULT,
		KEY_PRESS_UP,
		KEY_PRESS_DOWN,
		KEY_PRESS_LEFT,
		KEY_PRESS_RIGHT,
		KEY_PRESS_TOTAL
	};


	// min width and height of collision rectangles
	const int MIN_COLLISION_H = 12;
	const int MIN_COLLISION_W = 5;

	int velocity; // player velocity
	bool sideCollision[RECT_TOTAL]; // side of collision
	moveType movement = MOVE_NONE; // player movement

	// jump data members
	bool jumping = false;
	unsigned int jumpStart = 0;
	const unsigned int jumpDuration = 200;

	// textures for keypresses
	SDL_Texture *keypressTextures[KEY_PRESS_TOTAL];

public:
	playerObj(); // default constructor
	playerObj(const int &xPos, const int &yPos, const int &vel, const int &width, const int &height = -1);

	bool checkCollision(std::vector<gameObj*> &objVector); // check for collision

	void checkKeyState(); // check keystate
	void updatePhysics(); // update physics based on flags
};