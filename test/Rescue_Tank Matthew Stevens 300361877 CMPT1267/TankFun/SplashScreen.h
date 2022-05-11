#pragma once
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "Game.h"
#include <iostream>

class SplashScreen
{
public:
	SplashScreen(class Game* parentGame, class SDL_Renderer* gameRenderer);
	void draw(SDL_Renderer* gameRenderer);

private:	
	class Game* mGame;
	TTF_Font* mFont60;
	TTF_Font* mFont28;
	std::string mGameName = "Rescue Tank!";
	std::string mPromptText = "Press Any Key To Start!";
	SDL_Rect mGameNameTextureSize;
	SDL_Rect mPromptTextTextureSize;
	SDL_Texture* mGameNameTexture;
	SDL_Texture* mPromptTextTexture;
	const SDL_Color TEXT_COLOR{ 255,255,0,255 };
};

