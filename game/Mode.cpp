#include "Mode.hpp"

#include <iostream>
#include <glm/vec3.hpp>
#include <common/color.hpp>
#include <common/Leds.hpp>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

Mode::Mode(Leds &_leds)
	: leds(_leds)
{
	L = luaL_newstate();
	luaL_openlibs(L);

	lua_geti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);

	const luaL_Reg globalFunctions[] =
	{
		{"get_neighbors", luaGetNeighbors},
		{"get_sphere_position", luaGetSpherePosition},
		{"get_world_position", luaGetWorldPosition},
		{"hsv_to_rgb", luaHSVToRGB},
		{"set_color", luaSetColor},
		{nullptr, nullptr},
	};

	lua_pushlightuserdata(L, this);
	luaL_setfuncs(L, globalFunctions, 1);

	lua_pushinteger(L, Leds::Count);
	lua_setfield(L, -2, "Count");

	lua_createtable(L, Leds::Count, 0);
	for (int i = 0; i < Leds::Count; ++i)
	{
		lua_pushinteger(L, i + 1);
		lua_rawseti(L, -2, i + 1);
	}
	lua_setfield(L, -2, "indices");

	lua_pop(L, 1);
}

Mode::~Mode()
{
	lua_close(L);
}

bool Mode::loadScript()
{
	if (luaL_loadfilex(L, nullptr, nullptr))
	{
		std::cerr << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
		return false;
	}
	return pcall(L, 0, 0);
}

bool Mode::update(float dt)
{
	lua_getglobal(L, "update");
	lua_pushnumber(L, dt);
	return pcall(L, 1, 0);
}

bool Mode::pcall(lua_State *L, int nargs, int nresults)
{
	if (lua_pcall(L, nargs, nresults, 0))
	{
		std::cerr << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
		return false;
	}
	return true;
}

static Mode *getThis(lua_State *L, int up)
{
	return (Mode *)lua_touserdata(L, lua_upvalueindex(up));
}

static int getLedIndex(lua_State *L, int arg)
{
	int index = luaL_checkinteger(L, arg) - 1;
	luaL_argcheck(L, index >= 0 && index < Leds::Count, arg, "Index out of range")	;
	return index;
}

int Mode::luaGetNeighbors(lua_State *L)
{
	auto pThis = getThis(L, 1);
	auto index = getLedIndex(L, 1);
	auto &neighbors = pThis->leds.neighbors[index];
	lua_createtable(L, neighbors.size(), 0);
	int i = 1;
	for (auto &neighbor : neighbors)
	{
		lua_pushinteger(L, neighbor + 1);
		lua_rawseti(L, -2, i);
		++i;
	}
	return 1;
}

int Mode::luaGetSpherePosition(lua_State *L)
{
	auto pThis = getThis(L, 1);
	auto index = getLedIndex(L, 1);
	auto &position = pThis->leds.spherePositions[index];
	lua_pushnumber(L, position.x);
	lua_pushnumber(L, position.y);
	lua_pushnumber(L, position.z);
	return 3;
}

int Mode::luaGetWorldPosition(lua_State *L)
{
	auto pThis = getThis(L, 1);
	auto index = getLedIndex(L, 1);
	auto &position = pThis->leds.worldPositions[index];
	lua_pushnumber(L, position.x);
	lua_pushnumber(L, position.y);
	lua_pushnumber(L, position.z);
	return 3;
}

int Mode::luaHSVToRGB(lua_State *L)
{
	auto H = luaL_checknumber(L, 1);
	auto S = luaL_checknumber(L, 2);
	auto V = luaL_checknumber(L, 3);
	float R, G, B;
	HSVtoRGB(H, S, V, R, G, B);
	lua_pushnumber(L, R);
	lua_pushnumber(L, G);
	lua_pushnumber(L, B);
	return 3;
}

int Mode::luaSetColor(lua_State *L)
{
	auto pThis = getThis(L, 1);
	auto index = getLedIndex(L, 1);
	auto R = luaL_checknumber(L, 2);
	auto G = luaL_checknumber(L, 3);
	auto B = luaL_checknumber(L, 4);
	pThis->leds.colors[index] = RGBtoColor(R, G, B);
	return 0;
}
