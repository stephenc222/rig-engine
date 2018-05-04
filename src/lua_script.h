#ifndef LUA_SCRIPT_H
#define LUA_SCRIPT_H

#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"
#include "script.h"


class LuaScriptAPI: public Script {
  public:
    GameState gameState;
    lua_State* configScriptCtx;
    int loadScript(const char* filename);
    int loadConfigScript(const char* filename);
    int getInt(const char* varName);
    GameState getGameState();
    ~LuaScriptAPI();
    LuaScriptAPI();
    void LuaScriptError();

};

#endif