#pragma once

struct lua_State;
class Leds;

class Mode
{
public:
	Mode(Leds &leds);
	~Mode();

	bool loadScript();

	bool update(float dt);

private:
	lua_State *L;
	Leds &leds;
	static bool pcall(lua_State *L, int nargs, int nresults);

	static int luaGetNeighbors(lua_State *L);
	static int luaGetSpherePosition(lua_State *L);
	static int luaGetWorldPosition(lua_State *L);
	static int luaHSVToRGB(lua_State *L);
	static int luaSetColor(lua_State *L);
};
