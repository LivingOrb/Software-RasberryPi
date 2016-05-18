#pragma once

#include <stdint.h>

#include <glm/vec3.hpp>

extern "C"
{
#include <ws281x/ws2811.h>
}

class Leds
{
public:
	Leds();
	~Leds();

	bool initialize();

	void render();

private:
	ws2811_t ledstring;
	ws2811_led_t *ledColors;
	glm::vec3 *ledPositions;

	static const int Count;
};
