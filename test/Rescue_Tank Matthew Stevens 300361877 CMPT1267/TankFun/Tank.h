#pragma once

#include "SDL/SDL.h"
#include "Vector2.h"
#include "Projectile.h"
#include "Game.h"
#include <cmath>
#include <iostream>


class Tank
{
public:
	Tank(class Game* parentGame);
	int getAimAngle();
	void handleInput(const Uint8* state);
	void nudgeAimAngle(int degrees);
	void setAimAngle(int degrees);
	void updateState(float deltaTime);
	void shoot();
	void draw(SDL_Renderer* mRenderer);
	int getShotPower();

private:
	const int AIMSPEED = 100;
	const Vector2 TANKSIZE{ 30.0f,20.0f };
	const int GUNSIZE = 30;

	const float DEG_TO_RAD = M_PI / 180.0f;

	Game* mGame = nullptr;
	float mAimAngle = 90.0f;
	float mAimSpeed = 0.0f;
	Vector2 mAimVect{ 0.0f,0.0f };
	Vector2 tankPos { 1024.0f / 2.0f , 768.0f - 20.0f };
	Vector2 gunPt{ tankPos.x, tankPos.y - GUNSIZE - TANKSIZE.y /2};
	SDL_Color tankColour { 255,0,0,255 };
	const int crossH = 10;
	const int crossW = 4;

	float timeSinceShot = 0.0f;
	const float shootRate = 1.0f;
	bool mReqShot = false;
	bool mIsCharging = false;

	const int POWERSPEED = 200;
	const int SHOTPOWERMAX = 100;
	const int SHOTPOWERMIN = 10;
	const float SHOTMULTIPLIER = 18.0f;	//determined by trial and error
	int mShotPower = (SHOTPOWERMAX + SHOTPOWERMIN) / 2.0f;
	int mShotPowerDelta = 0;

	void updateAimAngle(float deltaTime);
	void updateShot(float deltaTime);

};

