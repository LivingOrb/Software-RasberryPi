#include "color.hpp"

#include <algorithm>
#include <cmath>

static float clamp(float x, float min, float max)
{
	return std::min(max, std::max(min, x));
}

uint32_t HSVtoColor(float H, float S, float V)
{
	float Hval = std::fmod(H * 6.0f, 6.0f);
	int sel = (int)Hval;
	float mod = Hval - sel;
	float v1 = V * (1.0f - S);
	float v2 = V * (1.0f - S * mod);
	float v3 = V * (1.0f - S * (1.0f - mod));
	float R, G, B;
	switch (sel + 1)
	{
	case 0:
		R = V;
		G = v1;
		B = v2;
		break;
	case 1:
		R = V;
		G = v3;
		B = v1;
		break;
	case 2:
		R = v2;
		G = V;
		B = v1;
		break;
	case 3:
		R = v1;
		G = V;
		B = v3;
		break;
	case 4:
		R = v1;
		G = v2;
		B = V;
		break;
	case 5:
		R = v3;
		G = v1;
		B = V;
		break;
	case 6:
		R = V;
		G = v1;
		B = v2;
		break;
	case 7:
		R = V;
		G = v3;
		B = v1;
		break;
	}
	return RGBtoColor(R, G, B);
}

uint32_t RGBtoColor(float R, float G, float B)
{
	return ((int)(clamp(R, 0.0f, 1.0f) * 255) << 16) |
		((int)(clamp(G, 0.0f, 1.0f) * 255) << 8) |
		((int)(clamp(B, 0.0f, 1.0f) * 255) << 0);
}
