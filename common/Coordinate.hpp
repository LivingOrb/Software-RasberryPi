#pragma once

#include <glm/vec3.hpp>

class Coordinate
{
public:
	static const float LerpRatio;

	short data;
	float smoothed;
	float normalized;

	float min;
	float max;

	Coordinate(float min, float max);

	void raw();
	void smooth(float dt);
};
