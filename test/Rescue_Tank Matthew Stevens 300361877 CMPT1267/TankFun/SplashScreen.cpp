#include "SplashScreen.h"

SplashScreen::SplashScreen(Game* parentGame, SDL_Renderer* gameRenderer) {
	
	mGame = parentGame;
	mFont60 = TTF_OpenFont("laserwolf.ttf", 60);
	mFont28 = TTF_OpenFont("laserwolf.ttf", 28);

	if (mFont60 == nullptr || mFont28 == nullptr)	// ...so we can check if window was updated and thus see if the window was created successfully
	{
		SDL_Log("Unable to load font: %s", SDL_GetError());
		return;
	}

	SDL_Surface* textSurface = TTF_RenderText_Solid(mFont60, mGameName.c_str(), TEXT_COLOR);
	mGameNameTexture = SDL_CreateTextureFromSurface(gameRenderer, textSurface);

	mGameNameTextureSize = { static_cast<int>(mGame->SCREEN_WIDTH / 2 - textSurface->w / 2),
						static_cast<int>(mGame->SCREEN_HEIGHT / 3 - textSurface->h / 2),
						textSurface->w,
						textSurface->h };

	textSurface = TTF_RenderText_Solid(mFont28, mPromptText.c_str(), TEXT_COLOR);
	mPromptTextTexture = SDL_CreateTextureFromSurface(gameRenderer, textSurface);

	mPromptTextTextureSize = { static_cast<int>(mGame->SCREEN_WIDTH / 2 - textSurface->w / 2),
						static_cast<int>(mGame->SCREEN_HEIGHT / 2 - textSurface->h / 2),
						textSurface->w,
						textSurface->h };

	SDL_FreeSurface(textSurface);
}

void SplashScreen::draw(SDL_Renderer* gameRenderer) {
	SDL_RenderCopy(gameRenderer, mGameNameTexture, NULL, &mGameNameTextureSize);
	SDL_RenderCopy(gameRenderer, mPromptTextTexture, NULL, &mPromptTextTextureSize);
}
