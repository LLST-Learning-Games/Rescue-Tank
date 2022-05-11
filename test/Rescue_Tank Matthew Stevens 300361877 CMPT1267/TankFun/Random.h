#pragma once

#include<ctime>
#include<cstdlib>

class Random
{
public:
	Random();
	int getRand(int min, int max);

private:
	float mSeed;
};

