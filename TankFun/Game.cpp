
#include "Game.h"



Game::Game()
{
	mWindow = nullptr;
	mRenderer = nullptr;
	mMusic = nullptr;
	mRescue = nullptr;
	mBlast = nullptr;
	mFont = nullptr;
	mScoreTexture = nullptr;
	mHighScoreTexture = nullptr;
	mPowerTexture = nullptr;
	mTicksCount=0;
	mIsRunning=true;
	mSplash = nullptr;

}

bool Game::Initialize()
{
	// Initialize SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	
	// Create an SDL Window
	mWindow = SDL_CreateWindow(
		"CMPT 1267", // Window title
		100,	// Top left x-coordinate of window
		100,	// Top left y-coordinate of window
		SCREEN_WIDTH,	// Width of window
		SCREEN_HEIGHT,	// Height of window
		0		// Flags (0 for no flags set)
	);

	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	
	// Create SDL renderer
	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create renderer for
		-1,		 // Usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}
	
	//Init graphics
	IMG_Init(IMG_INIT_PNG);

	//Init audio
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	//Init TrueType Fonts
	TTF_Init();

	//Load the font
	mFont = TTF_OpenFont("laserwolf.ttf", 28);
	if (mFont == nullptr)	// ...so we can check if window was updated and thus see if the window was created successfully
	{
		SDL_Log("Unable to load font: %s", SDL_GetError());
		return false;
	}

	//load audio
	mMusic = Mix_LoadMUS("TomaszKucza-FelicitousForest-loop1.wav");
	mRescue = Mix_LoadWAV("success.wav");
	mBlast = Mix_LoadWAV("fall.wav");
	//Play the music
	Mix_PlayMusic(mMusic, -1);

	// Create the custom random generator
	mRandomGenerator = new Random();

	// create the splash screen
	mSplash = new SplashScreen(this, mRenderer);

	// Set the background color (in case you ever want to change it... not currently used)
	mBGColor = GREEN_BG;
	
	// Make a tank
	mTank1 = new Tank(this);

	// Make the targets
	for (int i = 0; i < TARGNUM; i++)
		mTarg1[i] = new Target(this);

	// Make the projectiles, put them in the pool. They default to being inactive
	for (int i = 0; i < PROJECTILE_POOL_SIZE; i++)
		mProjectilePool[i] = new Projectile(this);

	//Init scores
	loadHighScore();
	resetGameStats();

	return true;
}

// Reset the things that need to be reset if you're starting a new game from the splash screen
void Game::resetGameStats() {
	funding = STARTINGFUNDING;
	score = 0;
	for (int i = 0; i < TARGNUM; i++)
		mTarg1[i]->randPos();
	updateScoreDisplay();
}

// The main game loop
void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			// If we get an SDL_QUIT event, end loop
			case SDL_QUIT:
				mIsRunning = false;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_z:
					if (Mix_PausedMusic() == 1)
					{
						//resume the music
						Mix_ResumeMusic();
					}
					//if the music is playing
					else
					{
						//pause the music
						Mix_PauseMusic();
					}
				}

				// if any key is pressed during the splash screen, start the game 
				if (mShowSplashScreen) {
					resetGameStats();
					SDL_RenderCopy(mRenderer, mFundingTexture, NULL, &mFundingTextureSize);
					mShowSplashScreen = false;
				}
		}
	}
	
	// Get state of keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);

	// If escape is pressed, also end loop
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	// If the splash screen is up, the tank doesn't receive input. 
	if (mShowSplashScreen) {
		return;
	}

	//Otherwise, the tank moves
	mTank1->handleInput(state);


}

void Game::UpdateGame()
{
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// Delta time is the difference in ticks from last frame
	// (converted to seconds)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// Clamp maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();
	
	// If the player is between games, show the splash screen
	if (mShowSplashScreen) {
		mSplash->draw(mRenderer);
		return;
	}

	// Update the power dispay info every frame
	updatePowerDisplay(mTank1->getShotPower());

	// Update the tank
	mTank1->updateState(deltaTime);
	
	// Update all the targets
	for (int i = 0; i < TARGNUM; i++)
		mTarg1[i]->updateState(deltaTime);
	
	// Update all the active projectiles in the projectile pool
	for (int i = 0; i < PROJECTILE_POOL_SIZE; i++) {
		if (mProjectilePool[i]->isActive()) {

			mProjectilePool[i]->updateProjectile(deltaTime);
			
			for (int j = 0; j < TARGNUM; j++)
				if(mProjectilePool[i]->checkCollision(mTarg1[j]))
					Mix_PlayChannel(-1, mRescue, 0);
		}
	}

	// Take funding from the player every second
	costOfDoingBusiness(deltaTime);
}




void Game::GenerateOutput()
{
	// Set draw color current background colour
	SDL_SetRenderDrawColor(
		mRenderer,
		mBGColor.r,		// R
		mBGColor.g,		// G 
		mBGColor.b,	// B
		mBGColor.a // A
	);
	
	// Clear back buffer
	SDL_RenderClear(mRenderer);

	// Draw walls
	SDL_SetRenderDrawColor(mRenderer, 
		WALL_COLOR.r, 
		WALL_COLOR.g, 
		WALL_COLOR.b, 
		WALL_COLOR.a);
	
	// Draw top wall
	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		SCREEN_WIDTH,		// Width
		WALL_THICKNESS	// Height
	};
	SDL_RenderFillRect(mRenderer, &wall);
	
	// Draw bottom wall
	wall.y = SCREEN_HEIGHT - WALL_THICKNESS;
	SDL_RenderFillRect(mRenderer, &wall);

	// Draw Scores
	SDL_RenderCopy(mRenderer, mScoreTexture, NULL, &mScoreTextureSize);
	SDL_RenderCopy(mRenderer, mHighScoreTexture, NULL, &mHighScoreTextureSize);
	SDL_RenderCopy(mRenderer, mPowerTexture, NULL, &mPowerTextureSize);
	SDL_RenderCopy(mRenderer, mFundingTexture, NULL, &mFundingTextureSize);

	// Draw the tank
	mTank1->draw(mRenderer);

	// Draw the targets
	for (int i = 0; i < TARGNUM; i++)
		mTarg1[i]->draw(mRenderer);

	// Draw the projectiles
	for (int i = 0; i < PROJECTILE_POOL_SIZE; i++) {
		if (mProjectilePool[i]->isActive()) {
			mProjectilePool[i]->draw(mRenderer);
		}
	}
	
	// If the player is between games, draw the splash screen
	if (mShowSplashScreen) {
		mSplash->draw(mRenderer);
	}

	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}


//void Game::addProjectile(Projectile* newProjToAdd) {
//	mProjectileList.push_back(newProjToAdd);
//}

Projectile* Game::getInactiveProjectile() {
	for (int i = 0; i < PROJECTILE_POOL_SIZE; i++) {
		if (!mProjectilePool[i]->isActive()) {
			return mProjectilePool[i];
		}
	}
	return nullptr;
}


void Game::updateFundingDisplay() {
	std::string textureText = FUNDINGTEXT + std::to_string(funding);
	SDL_Surface* textSurface = TTF_RenderText_Solid(mFont, textureText.c_str(), WALL_COLOR);

	mFundingTexture = SDL_CreateTextureFromSurface(mRenderer, textSurface);
	mFundingTextureSize = { static_cast<int>(POWER_DISPLAY_LOCATION.x),
						static_cast<int>(POWER_DISPLAY_LOCATION.y),
						textSurface->w,
						textSurface->h };

	SDL_FreeSurface(textSurface);
}


void Game::updatePowerDisplay(int power) {
	std::string textureText = POWERTEXT + std::to_string(power);
	SDL_Surface* textSurface = TTF_RenderText_Solid(mFont, textureText.c_str(), WALL_COLOR);

	mPowerTexture = SDL_CreateTextureFromSurface(mRenderer, textSurface);
	mPowerTextureSize = { static_cast<int>(POWER_DISPLAY_LOCATION.x),
						static_cast<int>(POWER_DISPLAY_LOCATION.y) - mPowerTextureSize.h,
						textSurface->w,
						textSurface->h };

	SDL_FreeSurface(textSurface);
}

void Game::updateScoreDisplay() {
	std::string textureText = SCORETEXT + std::to_string(score);
	SDL_Surface* textSurface = TTF_RenderText_Solid(mFont, textureText.c_str(), WALL_COLOR);

	mScoreTexture = SDL_CreateTextureFromSurface(mRenderer, textSurface);
	mScoreTextureSize = { static_cast<int>(SCORE_DISPLAY_LOCATION.x),
						static_cast<int>(SCORE_DISPLAY_LOCATION.y),
						textSurface->w,
						textSurface->h };

	textureText = HIGHSCORETEXT + std::to_string(highScore);
	textSurface = TTF_RenderText_Solid(mFont, textureText.c_str(), WALL_COLOR);

	mHighScoreTexture = SDL_CreateTextureFromSurface(mRenderer, textSurface);
	mHighScoreTextureSize = { static_cast<int>(SCORE_DISPLAY_LOCATION.x),
						static_cast<int>(SCORE_DISPLAY_LOCATION.y) - mScoreTextureSize.h,
						textSurface->w,
						textSurface->h };

	SDL_FreeSurface(textSurface);
}

void Game::costOfDoingBusiness(float deltaTime) {
	if (costTimer > COSTTIME)
	{
		addFunding(COSTOFDOINGBUSINESS);
		score += 1;

		if (score > highScore)
			highScore = score;

		updateScoreDisplay();
		costTimer = 0;
	}
	else
		costTimer += deltaTime;
}

void Game::addFunding(int amt) {
	funding += amt;
	updateFundingDisplay();

	if (funding < 0) {
		mShowSplashScreen = true;
	}
}

void Game::increaseScore() {
	funding++;
	if (funding > highScore)
		highScore = funding;

	updateScoreDisplay();
}

Random* Game::getRand() {
	return mRandomGenerator;
}

void Game::playBlastSound() {
	Mix_PlayChannel(-1, mBlast, 0);

}

void Game::saveHighScore() {
	saveHighScoreStream.open(HIGHSCORE_FILENAME);
	saveHighScoreStream << highScore;
	saveHighScoreStream.close();
}

void Game::loadHighScore() {
	loadHighScoreStream.open(HIGHSCORE_FILENAME);
	loadHighScoreStream >> highScore;
	loadHighScoreStream.close();
}



void Game::Shutdown()
{
	delete mTank1;
	delete mRandomGenerator;
	delete mSplash;

	// The array isn't dynamically allocated, but the elements within the array are.
	// That might be a mistake, but it works well enough at this point.
	for (int i = 0; i < PROJECTILE_POOL_SIZE; i++) {
		delete mProjectilePool[i];
	}
	
	for (int i = 0; i < TARGNUM; i++) {
		delete mTarg1[i];
	}

	saveHighScore();
	Mix_HaltMusic();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_DestroyTexture(mScoreTexture);
	SDL_DestroyTexture(mHighScoreTexture);
	Mix_FreeChunk(mRescue);
	Mix_FreeChunk(mBlast);
	Mix_FreeMusic(mMusic);
	SDL_Quit();
}