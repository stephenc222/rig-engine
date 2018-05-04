#ifndef LUA_SCRIPT_H
#define LUA_SCRIPT_H

#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"
#include <string>
#include "script.h"


class LuaScriptAPI: public Script {
  public:
    GameState gameState;
    lua_State* configScriptCtx;
    int loadScript(std::string& filename);
    int loadConfigScript(std::string& filename);
    int getInt(std::string& varName);
    GameState getGameState();
    ~LuaScriptAPI();
    LuaScriptAPI();
    void LuaScriptError();

};

#endif