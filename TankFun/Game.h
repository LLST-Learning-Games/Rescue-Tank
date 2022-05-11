#pragma once
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h"
#include "Tank.h"
#include "Projectile.h"
#include "Target.h"
#include "Vector2.h"
#include "Random.h"
#include "SplashScreen.h"
#include <string>
#include <fstream>
#include <list>


class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();
	//void addProjectile(class Projectile* newProjToAdd);
	class Projectile* getInactiveProjectile();
	class Random* getRand();
	void addFunding(int amt);
	void playBlastSound();

	static const int SCREEN_HEIGHT = 768;
	static const int SCREEN_WIDTH = 1024;

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	void resetGameStats();
	void increaseScore();
	void updateScoreDisplay();
	void updatePowerDisplay(int power);
	void updateFundingDisplay();
	void loadHighScore();
	void saveHighScore();
	void costOfDoingBusiness(float deltaTime);

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	SDL_Texture* mScoreTexture;
	SDL_Texture* mHighScoreTexture;
	SDL_Texture* mPowerTexture;
	SDL_Texture* mFundingTexture;
	TTF_Font* mFont;
	Uint32 mTicksCount;

	Mix_Music* mMusic;
	Mix_Chunk* mRescue;
	Mix_Chunk* mBlast;

	class SplashScreen* mSplash;

	int funding;
	int score;
	const int STARTINGFUNDING = 100;
	int highScore;
	float costTimer = 1;
	const float COSTTIME = 1;
	const int COSTOFDOINGBUSINESS = -1;


	class Tank* mTank1;
	//std::list<Projectile*> mProjectileList;

	const int PROJECTILE_POOL_SIZE = 20;
	class Projectile* mProjectilePool[20];

	const int TARGNUM = 3;
	class Target *mTarg1[3];	// This is a little sloppy... todo: figure out why I can't just use TARGNUM here. For now just make sure they're the same.

	class Random* mRandomGenerator;
	bool mIsRunning;
	bool mShowSplashScreen = true;
	SDL_Color mBGColor;

	SDL_Rect mScoreTextureSize;
	SDL_Rect mHighScoreTextureSize;
	SDL_Rect mFundingTextureSize;
	SDL_Rect mPowerTextureSize;
	const Vector2 SCORE_DISPLAY_LOCATION{ SCREEN_WIDTH - 500.0f, 100.0f };
	const Vector2 POWER_DISPLAY_LOCATION{ 100.0f, 100.0f };
	const std::string SCORETEXT = "Score: ";
	const std::string FUNDINGTEXT = "Funding: ";
	const std::string HIGHSCORETEXT = "High Score: ";
	const std::string POWERTEXT = "Shot Power: ";
	std::ofstream saveHighScoreStream;
	std::ifstream loadHighScoreStream;
	const std::string HIGHSCORE_FILENAME = "highscore.txt";

	const int WALL_THICKNESS = 15;
	const SDL_Color WALL_COLOR{ 255,255,0,255 };
	const SDL_Color GREEN_BG{ 0,50,0,255 };
	const SDL_Color RED_BG{ 100,0,0,255 };
	
};
