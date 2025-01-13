//+++++++++++++++++++++++++++++++++++++++++++++++++++++ V5 ++++++++++++++++++++++++++++++++++++++++++++++++++
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

using namespace std;

//+++++++++++++++++++++++++++++++++++++++++++++++ Include Lua headers +++++++++++++++++++++++++++++++++++++++
extern "C"
{
#include "Lua/include/lua.h"
#include "Lua/include/lauxlib.h"
#include "Lua/include/lualib.h"
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//+++++++++++++++++++++++++++++++++++++++++++++++++++++ Link Lua library for Windows ++++++++++++++++++++++++
#ifdef _WIN32
#pragma comment(lib, "Lua/lua54.lib")
#endif
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//+++++++++++++++++++++++++++++++++++++++++++++++++++++ Check for error msg +++++++++++++++++++++++++++++++++
bool CheckLua(lua_State * L, int r) {

	if (r != LUA_OK) {
		string errormsg = lua_tostring(L, -1);
		cout << errormsg << endl;
		return false;
	}
	return true;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


class GameWorld : public olc::PixelGameEngine
{
public:
	GameWorld() { sAppName = "Lua game"; }

	lua_State* script;

	olc::vi2d vTileSize = { 32,32 };
	//Graphics 
	olc::Renderable gfx;

	enum class TileType
	{
		Empty = 0,
		Solid
	};

	olc::vi2d vLevelSize;
	vector<TileType> vLevel;

public:
	bool OnUserCreate() override {
		gfx.Load("./gfx/actors.png");

		//Load virtual machine
		script = luaL_newstate();
		luaL_openlibs(script);

		// register name of host function
		lua_register(script, "_CreateLevel", wrap_CreateLevel);

		lua_register(script, "_SetTile", wrap_SetTile);

		if (CheckLua(script, luaL_dofile(script, "Luascript.lua"))){
			//go to stack
			lua_getglobal(script, "Loadlevel");
			if (lua_isfunction(script, -1)) {

				lua_pushlightuserdata(script, this);
				lua_pushnumber(script, 1);
				if (CheckLua(script,lua_pcall(script,2,1,0)))
				{
					return true;
				}
			}
		}

		return false;
	}

	//Display frames
	bool OnUserUpdate(float fElapsedTime) override {


		Clear(olc::CYAN);
		olc::vi2d vTile = { 0,0 };
		for (vTile.x = 0; vTile.x < vLevelSize.x; vTile.x++) {

			for (vTile.y = 0; vTile.y < vLevelSize.y; vTile.y++) {

				TileType b = vLevel[vTile.y * vLevelSize.x + vTile.x];

				switch (b)
				{
				case TileType::Empty:break;
				case TileType::Solid: DrawPartialSprite(vTile * vTileSize, 
					gfx.Sprite(), olc::vi2d(0, 0) * 
					vTileSize, vTileSize); break;

				}

			}

		}

		return true;
	}

	void CreateLevel(int w, int h)
	{
		vLevelSize = { w,h };
		vLevel.clear();
		vLevel.resize(w * h);
	}

	static int wrap_CreateLevel(lua_State* L) {
		if (lua_gettop(L) != 3) return -1;
		GameWorld* object = static_cast<GameWorld*>(lua_touserdata(L, 1));
		int w = lua_tointeger(L, 2);
		int h = lua_tointeger(L, 3);
		object->CreateLevel(w, h);
		return 0;
	}

	void SetTile(int x, int y, int tile) 
	{
		switch (tile)
		{
		case 0:
			vLevel[y * vLevelSize.x + x] = TileType::Empty; break;

		case 1:
			vLevel[y * vLevelSize.x + x] = TileType::Solid; break;
		}
	}

	static int wrap_SetTile(lua_State* L)
	{
		if (lua_gettop(L) != 4) return -1;
		GameWorld* object = static_cast<GameWorld*>(lua_touserdata(L, 1));
		int x = lua_tointeger(L, 2);
		int y = lua_tointeger(L, 3);
		int tile_id = lua_tointeger(L, 4);
		object->SetTile(x, y, tile_id);
		return 0;
	}

};

int main() {

	GameWorld demo;
	if (demo.Construct(512, 480, 2, 2))
		demo.Start();
	return 0;
}