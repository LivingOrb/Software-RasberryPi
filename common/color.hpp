#pragma once

#include <cstdint>

void HSVtoRGB(float H, float S, float V, float &R, float &G, float &B);
uint32_t RGBtoColor(float R, float G, float B);
