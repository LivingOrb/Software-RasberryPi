#pragma once

#include <cstdint>
#include <vector>

#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>

extern "C"
{
#include <ws281x/ws2811.h>
}

struct lua_State;

class Leds
{
public:
	bool initialize();
	void shutdown();

	void updateWorldPositions(const glm::mat3 &sphereToWorld);

	void clear();
	void render();

	std::size_t getCount() const;
	const std::vector<int> &getNeighbors(std::size_t led) const;
	const glm::vec3 &getSpherePosition(std::size_t led) const;
	const glm::vec3 &getWorldPosition(std::size_t led) const;
	void setColor(std::size_t led, uint32_t color);

private:
	ws2811_t ledstring;
	std::vector<std::vector<int>> neighbors;
	std::vector<glm::vec3> spherePositions;
	std::vector<glm::vec3> worldPositions;

	static Leds *luaGetThis(lua_State *L);
	static std::size_t luaGetLedIndex(const Leds *pThis, lua_State *L, int arg);

	static int luaSetCount(lua_State *L);
	static int luaAddNeighbor(lua_State *L);
	static int luaSetSpherePosition(lua_State *L);
};
