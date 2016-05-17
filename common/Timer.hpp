#pragma once

#include <sys/time.h>

class Timer
{
public:
	bool initialize();

	float getElapsedTime();

private:
	struct timeval lastTv;
	struct timezone uselessTz;
};
