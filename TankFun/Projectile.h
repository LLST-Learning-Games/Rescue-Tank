#pragma once

#include "SDL/SDL.h"
#include "Vector2.h"
#include "Game.h"
#include <iostream>

class Projectile
{

public:
	//Projectile(Vector2 v, Vector2 pos, float speed, class Game* parentGame);
	Projectile(class Game* parentGame);
	void updateProjectile(float deltaTime);
	void setVector(Vector2 v);
	void setPos(Vector2 origin);
	void updatePos(Vector2 pos);
	void draw(SDL_Renderer* mRenderer);
	bool checkCollision(class Target* chkTarget);
	void setActive();
	bool isActive();

private:
	Vector2 mVect {0.0f,0.0f};
	Vector2 mPos {0.0f,0.0f};
	//float mSpeed;
	const int SHOTSIZE = 5;
	float mGrav = 200.0f;
	class Game* mGame;
	bool mActive = false;
	SDL_Color mShotColour{ 255,0,0,255 };
};

