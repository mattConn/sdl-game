#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "global.h"

class bulletObj;

// any game object
// ===============
class gameObj {
private:

	SDL_Texture *currentTexture = nullptr;

	gameObj* bullet = nullptr;
	int duration = 0;
	int timeout = 0;

	SDL_Rect rect; // obj rect (used for coordinates)
	double velocity = 1;
	double velocityMod = 1;

	// starting position
	int initialX = 0;
	int initialY = 0;

public:
	// default constructor
	gameObj()
	{
		rect = global::makeRect(0, 0, 1, 1);
	}
	gameObj(SDL_Texture *t, const double& vel, const int &xPos, const int &yPos, const int &width, const int &height) : currentTexture(t), velocity(vel)
	{
		rect = global::makeRect(xPos, yPos, width, height);
		setInitialX(xPos);
		setInitialY(yPos);
	}
	// copy constructor
	gameObj(const gameObj& g) : currentTexture(g.currentTexture), velocity(g.velocity)
	{
		rect = g.rect;
	}

	// destructor
	~gameObj() 
	{
		SDL_DestroyTexture(currentTexture);
		currentTexture = nullptr;
	}

	// accessors
	SDL_Texture *getCurrentTexture()
	{
		return currentTexture;
	}

	void setCurrentTexture(SDL_Texture *t)
	{
		currentTexture = t;
	}

	bool isOffscreen() {
		if (getRectR() < 0 || getRectL() > global::SCREEN_WIDTH || getRectTop() > global::SCREEN_HEIGHT || getRectBottom() < 0)
		{
			DEBUG_MSG("Offscreen: gameObj &: " << this << " x: " << getRectX() << " y: " << getRectY());
			return true;
		}
		return false;
	}

	// get velocity
	double getVelocity() { return velocity; }
	double getVelocityMod() { return velocityMod; }

	void setVelocityMod(const double& v) { velocityMod = v; }

	// get rect sides
	int getRectTop(){ return rect.y; }
	int getRectBottom(){ return rect.y + rect.h; }
	int getRectL() { return rect.x; }
	int getRectR() { return rect.x + rect.w; }

	int getRectX() { return rect.x; }
	int getRectY() { return rect.y; }

	void setRectX(const int &x) { rect.x = x; }
	void setRectY(const int &y) { rect.y = y; }

	SDL_Rect getRect() const { return rect; }

	// get initial position
	int getInitialX() { return initialX; }
	int getInitialY() { return initialY; }
	
	// set initial position
	void setInitialX(const int& x) { initialX = x; };
	void setInitialY(const int& y) { initialY = y; };

	void setInitialPos(const int& x, const int& y)
	{
		setInitialX(x);
		setInitialY(y);
	}

	// get bullet
	gameObj* getNewBullet()
	{ 
		gameObj *newBullet = new gameObj(*bullet); // copy of bullet
		newBullet->setRectX(getRectX());
		newBullet->setRectY(getRectY());

		return newBullet;
	}

	// set bullet
	void setBullet(SDL_Texture* t, const double& vel, const int& width, const int& height, const int& d)
	{
		bullet = new gameObj(t, vel, 0, 0, width, height);
		bullet->duration = d;
		bullet->timeout = d + SDL_GetTicks();
	}

	// mutators
	void incRectX(const int n) { rect.x += n; }
	void incRectY(const int n) { rect.y += n; }

	void decRectX(const int n) { rect.x -= n; }
	void decRectY(const int n) { rect.y -= n; }

	void setDuration(const int& d) { duration = d; }
	void resetTimeout() { timeout = duration + SDL_GetTicks(); }

	// accessors
	gameObj* getBullet() { return bullet; }
	int getDuration() { return duration; }
	int getTimeout() { return timeout; }

};
