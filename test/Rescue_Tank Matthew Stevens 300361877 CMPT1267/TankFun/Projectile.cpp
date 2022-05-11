#include "Projectile.h"

//Projectile::Projectile(Vector2 v, Vector2 pos, float speed, Game* parentGame) {
//	setVector(v);
//	mSpeed = speed;
//	setPos(pos);
//	std::cout << "I live!\n";
//	mGame = parentGame;
//}

Projectile::Projectile(Game* parentGame) {
	setPos({0,0});
	// std::cout << "I live!\n";
	mGame = parentGame;
}

void Projectile::updateProjectile(float deltaTime) {
	
	if (isActive()) {
		mVect.y += mGrav * deltaTime;
		//mPos.x += mVect.x * mSpeed * deltaTime;
		//mPos.y += mVect.y * mSpeed * deltaTime;
		mPos.x += mVect.x * deltaTime;
		mPos.y += mVect.y * deltaTime;


		if (mPos.y > mGame->SCREEN_HEIGHT || mPos.x < 0 || mPos.x > mGame->SCREEN_WIDTH)
		{
			mActive = false;
			// std::cout << "I die...\n";
		}
	}
}

void Projectile::setActive() {
	// std::cout << "I live again!";
	mActive = true;
}
bool Projectile::isActive() {
	return mActive;
}

void Projectile::setVector(Vector2 v) {
	mVect = v;
}

void Projectile::setPos(Vector2 origin) {
	mPos = origin;
}

void Projectile::draw(SDL_Renderer* mRenderer) {
	if (!mActive)
		return;

	SDL_SetRenderDrawColor(mRenderer,
		mShotColour.r,
		mShotColour.g,
		mShotColour.b,
		mShotColour.a);

	SDL_Rect shotBox{
		static_cast<int>(mPos.x - SHOTSIZE / 2.0f),
		static_cast<int>(mPos.y - SHOTSIZE / 2.0f),
		SHOTSIZE,
		SHOTSIZE
	};

	SDL_RenderFillRect(mRenderer, &shotBox);
//	std::cout << "I am at: (" << mPos.x << "," << mPos.y << ")\n";
}


bool Projectile::checkCollision(class Target* chkTarget) {
	float targSize = chkTarget->getSize().x;
	float impactDistSq = SHOTSIZE * SHOTSIZE + targSize * targSize;
	
	// find actual distance here, then compare with impactDistSq
	float distSq = (mPos.x - chkTarget->getPos().x) * (mPos.x - chkTarget->getPos().x)
		+ (mPos.y - chkTarget->getPos().y) * (mPos.y - chkTarget->getPos().y);

	if (distSq < impactDistSq) {
		// std::cout << "kablam!";
		mGame->addFunding(chkTarget->getSize().x);
		chkTarget->randPos();
		return true;
	}
	return false;
}