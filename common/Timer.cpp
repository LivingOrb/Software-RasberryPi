#include "Timer.hpp"

bool Timer::initialize()
{
	return gettimeofday(&lastTv, &uselessTz) == 0;
}

float Timer::getElapsedTime()
{
	struct timeval tv;
	if (gettimeofday(&tv, &uselessTz))
		return 0.0f;
	float dt = (tv.tv_sec - lastTv.tv_sec) + 1e-6 * (tv.tv_usec - lastTv.tv_usec);
	lastTv = tv;
	return dt;
}
