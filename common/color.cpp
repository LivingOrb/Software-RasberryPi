#include "color.hpp"

#include <algorithm>
#include <cmath>

template <typename T>
static T clamp(T x, T min, T max)
{
	return std::min(max, std::max(min, x));
}

template <typename T>
static T wrap(T value, T max)
{
	return value - max * std::floor(value / max);
}

void HSVtoRGB(float H, float S, float V, float &R, float &G, float &B)
{
	float Hval = wrap(H * 6.0f, 6.0f);
	int sel = (int)Hval;
	float mod = Hval - sel;
	float v1 = V * (1.0f - S);
	float v2 = V * (1.0f - S * mod);
	float v3 = V * (1.0f - S * (1.0f - mod));
	switch (sel % 6)
	{
	case 0:
		R = V;
		G = v3;
		B = v1;
		break;
	case 1:
		R = v2;
		G = V;
		B = v1;
		break;
	case 2:
		R = v1;
		G = V;
		B = v3;
		break;
	case 3:
		R = v1;
		G = v2;
		B = V;
		break;
	case 4:
		R = v3;
		G = v1;
		B = V;
		break;
	case 5:
		R = V;
		G = v1;
		B = v2;
		break;
	}
}

uint32_t RGBtoColor(float R, float G, float B)
{
	return (clamp((uint32_t)(R * 256.f), 0u, 255u) << 16) |
		(clamp((uint32_t)(G * 256.f), 0u, 255u) << 8) |
		(clamp((uint32_t)(B * 256.f), 0u, 255u) << 0);
}
