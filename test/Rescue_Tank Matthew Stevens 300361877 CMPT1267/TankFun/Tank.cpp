#include "Tank.h"


Tank::Tank(Game* parentGame) {
	mGame = parentGame;
}
int Tank::getAimAngle() {
	return mAimAngle;
}

void Tank::nudgeAimAngle(int degrees) {
	mAimSpeed = degrees;
}

void Tank::setAimAngle(int degrees) {
	mAimAngle = degrees;
}

void Tank::updateState(float deltaTime) {
	updateAimAngle(deltaTime);
	updateShot(deltaTime);
}

void Tank::updateAimAngle(float deltaTime) {

	mAimAngle += (mAimSpeed * deltaTime);

	if (mAimAngle > 180)
		mAimAngle = 180;
	else if (mAimAngle < 0)
		mAimAngle = 0;

	gunPt.x = -cos(mAimAngle * DEG_TO_RAD) * GUNSIZE + tankPos.x;
	gunPt.y = -sin(mAimAngle * DEG_TO_RAD) * GUNSIZE + tankPos.y;
	
}

void Tank::updateShot(float deltaTime) {
	// Update shot power
	mShotPower += mShotPowerDelta * deltaTime;
	if (mShotPower > SHOTPOWERMAX)
		mShotPower = SHOTPOWERMAX;
	else if (mShotPower < SHOTPOWERMIN)
		mShotPower = SHOTPOWERMIN;
	mShotPowerDelta = 0;

	// Update shot cooldown timer 
	//if (mReqShot && timeSinceShot > shootRate) {
	//	shoot();
	//	timeSinceShot = 0.0f;
	//}
	//timeSinceShot += deltaTime;
}

void Tank::handleInput(const Uint8* state) {
	mAimSpeed = 0.0f;
	if (state[SDL_SCANCODE_D])
	{
		nudgeAimAngle(AIMSPEED);
	}
	if (state[SDL_SCANCODE_A])
	{
		nudgeAimAngle(-AIMSPEED);
	}
	/*if (state[SDL_SCANCODE_W]) {
		mShotPowerDelta += POWERSPEED;
	}
	if (state[SDL_SCANCODE_S]) {
		mShotPowerDelta -= POWERSPEED;
	}*/
	if (state[SDL_SCANCODE_SPACE])
	{
		//if (timeSinceShot > shootRate / 2.0f)
		//	mReqShot = true;
		mIsCharging = true;

	}
	else {	// if space-bar isn't down, check if we're charging. if so, shoot once. 
		if(mIsCharging)
			shoot();

		mIsCharging = false;
	}

	if (mIsCharging) {
		// std::cout << "I'm charging!";
		mShotPowerDelta += POWERSPEED;
	}
	

}

void Tank::draw(SDL_Renderer* mRenderer) {


	
	SDL_Rect tankbody{
		static_cast<int>(tankPos.x - TANKSIZE.x / 2.0f),
		static_cast<int>(tankPos.y - TANKSIZE.y / 2.0f),
		TANKSIZE.x,
		TANKSIZE.y
	};


	SDL_Rect tankCrossH{
		static_cast<int>(tankPos.x - crossW / 2),
		static_cast<int>(tankPos.y - crossH / 2),
		crossW,
		crossH
	};

	SDL_Rect tankCrossW{
		static_cast<int>(tankPos.x - crossH / 2),
		static_cast<int>(tankPos.y - crossW / 2),
		crossH,
		crossW
	};

	SDL_SetRenderDrawColor(mRenderer,
		tankColour.r,
		tankColour.g,
		tankColour.b,
		tankColour.a);
	SDL_RenderDrawLine(mRenderer, tankPos.x, tankPos.y, gunPt.x, gunPt.y);
	SDL_RenderFillRect(mRenderer, &tankbody);

	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	SDL_RenderFillRect(mRenderer, &tankCrossH);
	SDL_RenderFillRect(mRenderer, &tankCrossW);

}


void Tank::shoot() {
	// std::cout << "BLAM!";
	Vector2 tempV{ 
		(gunPt.x - tankPos.x) * mShotPower / SHOTPOWERMAX * SHOTMULTIPLIER,
		(gunPt.y - tankPos.y) * mShotPower / SHOTPOWERMAX * SHOTMULTIPLIER };
	/*
	Projectile* newProj = new Projectile(tempV ,tankPos,10.0f,mGame);
	mGame->addProjectile(newProj);
	*/
	Projectile* newProj = nullptr;
	newProj = mGame->getInactiveProjectile();
	if (newProj != nullptr) {
		newProj->setPos(tankPos);
		newProj->setVector(tempV);
		newProj->setActive();
		mGame->addFunding(-mShotPower / 10);
		mGame->playBlastSound();
		//mReqShot = false;
	}
	mShotPower = SHOTPOWERMIN;
}

int Tank::getShotPower() {
	return mShotPower;
}