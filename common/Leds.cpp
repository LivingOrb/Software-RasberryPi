#include "Leds.hpp"

#include <cmath>
#include <cstring>
#include <iostream>

#include <glm/geometric.hpp>
#include <glm/gtx/string_cast.hpp>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

bool Leds::initialize()
{
	bool failed = false;
	auto L = luaL_newstate();
	luaL_openlibs(L);

	lua_geti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);

	const luaL_Reg globalFunctions[] =
	{
		{"set_count", luaSetCount},
		{"add_neighbor", luaAddNeighbor},
		{"set_sphere_position", luaSetSpherePosition},
		{nullptr, nullptr},
	};

	lua_pushlightuserdata(L, this);
	luaL_setfuncs(L, globalFunctions, 1);

	lua_pop(L, 1);

	if (luaL_loadfile(L, "leds.lua"))
	{
		std::cerr << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
		failed = true;
	}
	else if (lua_pcall(L, 0, 0, 0))
	{
		std::cerr << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
		failed = true;
	}

	lua_close(L);

	if (failed)
	{
		return false;
	}

	memset((char*)&ledstring, 0, sizeof(ledstring));
	ledstring.freq = WS2811_TARGET_FREQ;
	ledstring.dmanum = 5;

	ledstring.channel[0].gpionum = 18;
	ledstring.channel[0].count = getCount();
	ledstring.channel[0].invert = 1;
	ledstring.channel[0].brightness = 255;
	ledstring.channel[0].strip_type = WS2811_STRIP_GRB;

	if (ws2811_init(&ledstring))
	{
		return false;
	}

	return true;
}

void Leds::shutdown()
{
	clear();
	render();
	
	ws2811_fini(&ledstring);
}

void Leds::updateWorldPositions(const glm::mat3 &sphereToWorld)
{
	for (std::size_t i = 0; i < getCount(); ++i)
	{
		worldPositions[i] = sphereToWorld * spherePositions[i];
	}
}

void Leds::clear()
{
	memset((char*)ledstring.channel[0].leds, 0, getCount() * sizeof(ws2811_led_t));
}

void Leds::render()
{
	ws2811_render(&ledstring);
}

std::size_t Leds::getCount() const
{
	return neighbors.size();
}

const std::vector<int> &Leds::getNeighbors(std::size_t led) const
{
	return neighbors[led];
}

const glm::vec3 &Leds::getSpherePosition(std::size_t led) const
{
	return spherePositions[led];
}

const glm::vec3 &Leds::getWorldPosition(std::size_t led) const
{
	return worldPositions[led];
}

void Leds::setColor(std::size_t led, uint32_t color)
{
	if (led < getCount())
	{
		ledstring.channel[0].leds[led] = color;
	}
}

Leds *Leds::luaGetThis(lua_State *L)
{
	return (Leds *)lua_touserdata(L, lua_upvalueindex(1));
}

std::size_t Leds::luaGetLedIndex(const Leds *pThis, lua_State *L, int arg)
{
	std::size_t index = luaL_checkinteger(L, arg) - 1;
	luaL_argcheck(L, index >= 0 && index < pThis->getCount(), arg, "Index out of range");
	return index;
}

int Leds::luaSetCount(lua_State *L)
{
	auto pThis = luaGetThis(L);
	auto count = (std::size_t)luaL_checkinteger(L, 1);

	pThis->neighbors.resize(count);
	pThis->spherePositions.resize(count);
	pThis->worldPositions.resize(count);

	return 0;
}

int Leds::luaAddNeighbor(lua_State *L)
{

}

int Leds::luaSetSpherePosition(lua_State *L)
{

}
