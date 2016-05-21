#pragma once

#include <cstdint>
#include <vector>

#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>

extern "C"
{
#include <ws281x/ws2811.h>
}

class Leds
{
public:
	ws2811_led_t *colors;
	std::vector<std::vector<int>> neighbors;
	std::vector<glm::vec3> spherePositions;
	std::vector<glm::vec3> worldPositions;

	Leds();
	~Leds();

	bool initialize();

	void updateWorldPositions(const glm::mat3 &sphereToWorld);

	void clear();
	void render();

	static const int Count;
	static const float VertexAngle;

private:
	ws2811_t ledstring;

	void setVertex(int led, const glm::vec3 &position, int neighbor0, int neighbor1, int neighbor2, int neighbor3, int neighbor4);
	void setEdge(int led, const glm::vec3 &position0, const glm::vec3 &position1, float t, int neighbor0, int neighbor1);
};
