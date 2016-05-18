#include "Leds.hpp"

#include <cmath>
#include <cstring>
#include <iostream>

#include <glm/geometric.hpp>
#include <glm/gtx/string_cast.hpp>

const int Leds::Count = 102;
const float Leds::VertexAngle = atan2(0.894425f, 0.447215f);

Leds::Leds()
	: colors(nullptr)
	, spherePositions(Count)
	, neighbors(Count)
	, worldPositions(Count)
{}

Leds::~Leds()
{
	if (colors)
	{
		clear();
		ws2811_fini(&ledstring);
	}
}

bool Leds::initialize()
{
	memset((char*)&ledstring, 0, sizeof(ledstring));
	ledstring.freq = WS2811_TARGET_FREQ;
	ledstring.dmanum = 5;

	ledstring.channel[0].gpionum = 18;
	ledstring.channel[0].count = Count;
	ledstring.channel[0].invert = 1;
	ledstring.channel[0].brightness = 255;
	ledstring.channel[0].strip_type = WS2811_STRIP_GRB;

	if (ws2811_init(&ledstring))
	{
		return false;
	}

	colors = ledstring.channel[0].leds;

	std::vector<glm::vec3> vertices(12);
	vertices[0] = glm::vec3(0.0f, 0.0f, 1.0f);
	vertices[1] = glm::vec3(0.894425f, 0.0f, 0.447215f);
	vertices[2] = glm::vec3(0.276385f, 0.85064f, 0.447215f);
	vertices[3] = glm::vec3(-0.7236f, 0.52572f, 0.447215f);
	vertices[4] = glm::vec3(-0.7236f, -0.52572f, 0.447215f);
	vertices[5] = glm::vec3(0.276385f, -0.85064f, 0.447215f);
	vertices[6] = glm::vec3(0.0f, 0.0f, -1.0f);
	vertices[7] = glm::vec3(-0.894425f, 0.0f, -0.447215f);
	vertices[8] = glm::vec3(-0.276385f, -0.85064f, -0.447215f);
	vertices[9] = glm::vec3(0.7236f, -0.52572f, -0.447215f);
	vertices[10] = glm::vec3(0.7236f, 0.52572f, -0.447215f);
	vertices[11] = glm::vec3(-0.276385f, 0.85064f, -0.447215f);

	setVertex(0, vertices[0], 1, 6, 7, 12, 13);
	setEdge(1, vertices[0], vertices[1], 0.25f, 0, 2);
	setEdge(2, vertices[0], vertices[1], 0.5f, 1, 3);
	setEdge(3, vertices[0], vertices[1], 0.75f, 2, 20);

	setEdge(4, vertices[2], vertices[0], 0.25f, 24, 5);
	setEdge(5, vertices[2], vertices[0], 0.5f, 4, 6);
	setEdge(6, vertices[2], vertices[0], 0.75f, 5, 0);

	setEdge(7, vertices[0], vertices[3], 0.25f, 0, 8);
	setEdge(8, vertices[0], vertices[3], 0.5f, 7, 9);
	setEdge(9, vertices[0], vertices[3], 0.75f, 8, 28);

	setEdge(10, vertices[4], vertices[0], 0.25f, 32, 11);
	setEdge(11, vertices[4], vertices[0], 0.5f, 10, 12);
	setEdge(12, vertices[4], vertices[0], 0.75f, 11, 0);

	setEdge(13, vertices[0], vertices[5], 0.25f, 0, 14);
	setEdge(14, vertices[0], vertices[5], 0.5f, 13, 15);
	setEdge(15, vertices[0], vertices[5], 0.75f, 14, 16);

	setVertex(16, vertices[5], 15, 17, 35, 36, 65);
	setEdge(17, vertices[5], vertices[1], 0.25f, 16, 18);
	setEdge(18, vertices[5], vertices[1], 0.5f, 17, 19);
	setEdge(19, vertices[5], vertices[1], 0.75f, 18, 20);

	setVertex(20, vertices[1], 3, 19, 21, 41, 42);
	setEdge(21, vertices[1], vertices[2], 0.25f, 20, 22);
	setEdge(22, vertices[1], vertices[2], 0.5f, 21, 23);
	setEdge(23, vertices[1], vertices[2], 0.75f, 22, 24);

	setVertex(24, vertices[2], 4, 23, 25, 47, 48);
	setEdge(25, vertices[2], vertices[3], 0.25f, 24, 26);
	setEdge(26, vertices[2], vertices[3], 0.5f, 25, 27);
	setEdge(27, vertices[2], vertices[3], 0.75f, 26, 28);

	setVertex(28, vertices[3], 9, 27, 29, 53, 54);
	setEdge(29, vertices[3], vertices[4], 0.25f, 28, 30);
	setEdge(30, vertices[3], vertices[4], 0.5f, 29, 31);
	setEdge(31, vertices[3], vertices[4], 0.75f, 30, 32);

	setVertex(32, vertices[4], 10, 31, 33, 59, 60);
	setEdge(33, vertices[4], vertices[5], 0.25f, 32, 34);
	setEdge(34, vertices[4], vertices[5], 0.5f, 33, 35);
	setEdge(35, vertices[4], vertices[5], 0.75f, 34, 16);

	setEdge(36, vertices[5], vertices[9], 0.25f, 16, 37);
	setEdge(37, vertices[5], vertices[9], 0.5f, 36, 38);
	setEdge(38, vertices[5], vertices[9], 0.75f, 37, 82);

	setEdge(39, vertices[9], vertices[1], 0.25f, 82, 40);
	setEdge(40, vertices[9], vertices[1], 0.5f, 39, 41);
	setEdge(41, vertices[9], vertices[1], 0.75f, 40, 20);

	setEdge(42, vertices[1], vertices[10], 0.25f, 20, 43);
	setEdge(43, vertices[1], vertices[10], 0.5f, 42, 44);
	setEdge(44, vertices[1], vertices[10], 0.75f, 43, 78);

	setEdge(45, vertices[10], vertices[2], 0.25f, 78, 46);
	setEdge(46, vertices[10], vertices[2], 0.5f, 45, 47);
	setEdge(47, vertices[10], vertices[2], 0.75f, 46, 24);

	setEdge(48, vertices[2], vertices[11], 0.25f, 24, 49);
	setEdge(49, vertices[2], vertices[11], 0.5f, 48, 50);
	setEdge(50, vertices[2], vertices[11], 0.75f, 49, 74);

	setEdge(51, vertices[11], vertices[3], 0.25f, 74, 52);
	setEdge(52, vertices[11], vertices[3], 0.5f, 51, 53);
	setEdge(53, vertices[11], vertices[3], 0.75f, 52, 28);

	setEdge(54, vertices[3], vertices[7], 0.25f, 28, 55);
	setEdge(55, vertices[3], vertices[7], 0.5f, 54, 56);
	setEdge(56, vertices[3], vertices[7], 0.75f, 55, 70);

	setEdge(57, vertices[7], vertices[4], 0.25f, 70, 58);
	setEdge(58, vertices[7], vertices[4], 0.5f, 57, 59);
	setEdge(59, vertices[7], vertices[4], 0.75f, 58, 32);

	setEdge(60, vertices[4], vertices[8], 0.25f, 32, 61);
	setEdge(61, vertices[4], vertices[8], 0.5f, 60, 62);
	setEdge(62, vertices[4], vertices[8], 0.75f, 61, 66);

	setEdge(63, vertices[8], vertices[5], 0.25f, 66, 64);
	setEdge(64, vertices[8], vertices[5], 0.5f, 63, 65);
	setEdge(65, vertices[8], vertices[5], 0.75f, 64, 16);

	setVertex(66, vertices[8], 62, 63, 67, 85, 86);
	setEdge(67, vertices[8], vertices[7], 0.25f, 66, 68);
	setEdge(68, vertices[8], vertices[7], 0.5f, 67, 69);
	setEdge(69, vertices[8], vertices[7], 0.75f, 68, 70);

	setVertex(70, vertices[7], 56, 57, 69, 71, 91);
	setEdge(71, vertices[7], vertices[11], 0.25f, 70, 72);
	setEdge(72, vertices[7], vertices[11], 0.5f, 71, 73);
	setEdge(73, vertices[7], vertices[11], 0.75f, 72, 74);

	setVertex(74, vertices[11], 50, 51, 73, 75, 92);
	setEdge(75, vertices[11], vertices[10], 0.25f, 74, 76);
	setEdge(76, vertices[11], vertices[10], 0.5f, 75, 77);
	setEdge(77, vertices[11], vertices[10], 0.75f, 76, 78);

	setVertex(78, vertices[10], 44, 45, 77, 79, 97);
	setEdge(79, vertices[10], vertices[9], 0.25f, 78, 80);
	setEdge(80, vertices[10], vertices[9], 0.5f, 79, 81);
	setEdge(81, vertices[10], vertices[9], 0.75f, 80, 82);

	setVertex(82, vertices[9], 38, 39, 81, 83, 98);
	setEdge(83, vertices[9], vertices[8], 0.25f, 82, 84);
	setEdge(84, vertices[9], vertices[8], 0.5f, 83, 85);
	setEdge(85, vertices[9], vertices[8], 0.75f, 84, 66);

	setEdge(86, vertices[8], vertices[6], 0.25f, 66, 87);
	setEdge(87, vertices[8], vertices[6], 0.5f, 86, 88);
	setEdge(88, vertices[8], vertices[6], 0.75f, 87, 101);

	setEdge(89, vertices[6], vertices[7], 0.25f, 101, 90);
	setEdge(90, vertices[6], vertices[7], 0.5f, 89, 91);
	setEdge(91, vertices[6], vertices[7], 0.75f, 90, 70);

	setEdge(92, vertices[11], vertices[6], 0.25f, 74, 93);
	setEdge(93, vertices[11], vertices[6], 0.5f, 92, 94);
	setEdge(94, vertices[11], vertices[6], 0.75f, 93, 101);

	setEdge(95, vertices[6], vertices[10], 0.25f, 101, 96);
	setEdge(96, vertices[6], vertices[10], 0.5f, 95, 97);
	setEdge(97, vertices[6], vertices[10], 0.75f, 96, 78);

	setEdge(98, vertices[9], vertices[6], 0.25f, 82, 99);
	setEdge(99, vertices[9], vertices[6], 0.5f, 98, 100);
	setEdge(100, vertices[9], vertices[6], 0.75f, 99, 101);
	setVertex(101, vertices[6], 88, 89, 94, 95, 100);

	return true;
}

void Leds::updateWorldPositions(const glm::mat3 &sphereToWorld)
{
	for (int i = 0; i < Count; ++i)
	{
		worldPositions[i] = sphereToWorld * spherePositions[i];
	}
}

void Leds::render()
{
	ws2811_render(&ledstring);
}

void Leds::clear()
{
	memset((char*)colors, 0, Count * sizeof(ws2811_led_t));
	render();
}

void Leds::setVertex(int led, const glm::vec3 &position, int neighbor0, int neighbor1, int neighbor2, int neighbor3, int neighbor4)
{
	spherePositions[led] = glm::normalize(position);

	std::vector<int> &ledNeighbors = neighbors[led];
	ledNeighbors.reserve(5);
	ledNeighbors.push_back(neighbor0);
	ledNeighbors.push_back(neighbor1);
	ledNeighbors.push_back(neighbor2);
	ledNeighbors.push_back(neighbor3);
	ledNeighbors.push_back(neighbor4);
}

void Leds::setEdge(int led, const glm::vec3 &position0, const glm::vec3 &position1, float t, int neighbor0, int neighbor1)
{
	spherePositions[led] = glm::normalize(position0 * sinf((1 - t) * VertexAngle) + position1 * sinf(t * VertexAngle));

	std::vector<int> &ledNeighbors = neighbors[led];
	ledNeighbors.reserve(2);
	ledNeighbors.push_back(neighbor0);
	ledNeighbors.push_back(neighbor1);
}
