#include "Random.h"


Random::Random() {
	mSeed = time(0);
	srand(time(0));
}

int Random::getRand(int min, int max) {
	return rand() % (max - min) + min;
}