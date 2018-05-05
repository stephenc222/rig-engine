#ifndef LUA_SCRIPT_H
#define LUA_SCRIPT_H

#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"
#include "script.h"

struct ConfigScriptData {
  int width;
  int height;
  int useFullScreen;
};


class LuaScriptAPI: public Script {
  public:
    GameState gameState;
    lua_State* configScriptCtx;
    ConfigScriptData configScriptData;
    int loadScript(const char* filename);
    int loadConfigScript(const char* filename);
    void setConfigScriptData(int width, int height, int useFullScreen);
    ConfigScriptData& getConfigScriptData();
    int getInt(const char* varName);
    int getFieldInt(const char* varName);
    GameState getGameState();
    ~LuaScriptAPI();
    LuaScriptAPI();
    void LuaScriptError();

};

#endif