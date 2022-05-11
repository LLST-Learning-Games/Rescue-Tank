#pragma once


#include "SDL/SDL.h"
#include "Vector2.h"
#include "Game.h"
#include "Random.h"
#include <iostream>


class Target {

public:
	Target(class Game* parentGame);
	void updateState(float deltaTime);
	void draw(SDL_Renderer* mRenderer);
	Vector2 getPos();
	Vector2 getSize();
	void randPos();


private:
	class Game* mGame = nullptr;
	Vector2 mPos;
	Vector2 mVel;
	SDL_Color TARGSTARTCOLOUR{ 255,0 ,255,255 };
	SDL_Color targColour;
	const Vector2 TARGSTARTSIZE{ 30.0f,30.0f };
	const int MINSIZE = 5;
	Vector2 currentTargSize;
	const int SHRINKRATE = 2;
	const float FADERATE = 0.01f;
	const int DEATHPENALTY = -10;
	const int UPPERBOUND = 200;
	const int LOWERBOUND = 50;
	const int SIDEBOUND = 50;
};