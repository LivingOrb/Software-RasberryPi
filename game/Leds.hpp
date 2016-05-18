#pragma once

#include <cstdint>
#include <vector>

#include <glm/vec3.hpp>

extern "C"
{
#include <ws281x/ws2811.h>
}

class Leds
{
public:
	ws2811_led_t *ledColors;
	std::vector<glm::vec3> ledPositions;
	std::vector<std::vector<int>> ledNeighbors;

	Leds();
	~Leds();

	bool initialize();

	void render();
	void clear();

	static const int Count;
	static const float VertexAngle;

private:
	ws2811_t ledstring;

	void setVertex(int led, const glm::vec3 &position, int neighbor0, int neighbor1, int neighbor2, int neighbor3, int neighbor4);
	void setEdge(int led, const glm::vec3 &position0, const glm::vec3 &position1, float t, int neighbor0, int neighbor1);
};
