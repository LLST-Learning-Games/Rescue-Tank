#include "Target.h"

Target::Target(Game* parentGame) {
	mGame = parentGame;
	//mPos.x = mGame->SCREEN_WIDTH / 2;
	//mPos.y = mGame->SCREEN_HEIGHT / 2;
	randPos();
}

void Target::updateState(float deltaTime) {
	currentTargSize.x -= SHRINKRATE * deltaTime;
	currentTargSize.y -= SHRINKRATE * deltaTime;

	targColour.r = TARGSTARTCOLOUR.r * (currentTargSize.x / TARGSTARTSIZE.x);
	targColour.b = TARGSTARTCOLOUR.b * (currentTargSize.x / TARGSTARTSIZE.x);
	targColour.g = TARGSTARTCOLOUR.g * (currentTargSize.x / TARGSTARTSIZE.x);

	if (currentTargSize.x < MINSIZE) {
		randPos();
		mGame->addFunding(DEATHPENALTY);
	}
}

void Target::draw(SDL_Renderer* mRenderer) {
	SDL_SetRenderDrawColor(mRenderer,
		targColour.r,
		targColour.g,
		targColour.b,
		targColour.a);

	SDL_Rect targbody{
		static_cast<int>(mPos.x - currentTargSize.x / 2.0f),
		static_cast<int>(mPos.y - currentTargSize.y / 2.0f),
		currentTargSize.x,
		currentTargSize.y
	};


	SDL_RenderFillRect(mRenderer, &targbody);
}

Vector2 Target::getPos() {
	return mPos;
}

Vector2 Target::getSize() {
	return currentTargSize;
}

void Target::randPos() {
	currentTargSize = TARGSTARTSIZE;
	targColour = TARGSTARTCOLOUR;

	Random* targRand = mGame->getRand();

	mPos.x = targRand->getRand(SIDEBOUND, mGame->SCREEN_WIDTH - SIDEBOUND);
	mPos.y = targRand->getRand(UPPERBOUND, mGame->SCREEN_HEIGHT - LOWERBOUND);

}