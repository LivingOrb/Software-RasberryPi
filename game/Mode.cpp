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

	std::size_t count = leds.getCount();

	lua_pushinteger(L, count);
	lua_setfield(L, -2, "Count");

	lua_createtable(L, count, 0);
	for (std::size_t i = 0; i < count; ++i)
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

Mode *Mode::luaGetThis(lua_State *L)
{
	return (Mode *)lua_touserdata(L, lua_upvalueindex(1));
}

std::size_t Mode::luaGetLedIndex(const Mode *pThis, lua_State *L, int arg)
{
	std::size_t index = luaL_checkinteger(L, arg) - 1;
	luaL_argcheck(L, index < pThis->leds.getCount(), arg, "Index out of range");
	return index;
}

int Mode::luaGetNeighbors(lua_State *L)
{
	auto pThis = luaGetThis(L);
	auto index = luaGetLedIndex(pThis, L, 1);
	auto &neighbors = pThis->leds.getNeighbors(index);
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
	auto pThis = luaGetThis(L);
	auto index = luaGetLedIndex(pThis, L, 1);
	auto &position = pThis->leds.getSpherePosition(index);
	lua_pushnumber(L, position.x);
	lua_pushnumber(L, position.y);
	lua_pushnumber(L, position.z);
	return 3;
}

int Mode::luaGetWorldPosition(lua_State *L)
{
	auto pThis = luaGetThis(L);
	auto index = luaGetLedIndex(pThis, L, 1);
	auto &position = pThis->leds.getWorldPosition(index);
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
	auto pThis = luaGetThis(L);
	auto index = luaGetLedIndex(pThis, L, 1);
	auto R = luaL_checknumber(L, 2);
	auto G = luaL_checknumber(L, 3);
	auto B = luaL_checknumber(L, 4);
	pThis->leds.setColor(index, RGBtoColor(R, G, B));
	return 0;
}
