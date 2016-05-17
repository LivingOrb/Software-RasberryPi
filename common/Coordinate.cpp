#include "Coordinate.hpp"

static float lerp(float a, float b, float t)
{
	if (t > 1)
		t = 1;
	return a + (b - a) * t;
}

const float Coordinate::LerpRatio = 5.0f;

Coordinate::Coordinate(float _min, float _max)
	: min(_min)
	, max(_max)
{}

void Coordinate::raw()
{
	smoothed = data;
}

void Coordinate::smooth(float dt)
{
	smoothed = lerp(smoothed, data, LerpRatio * dt);
	normalized = (smoothed - min) / (max - min) * 2.0f - 1.0f;
}
