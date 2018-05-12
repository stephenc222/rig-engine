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

// this is where game scripting functions like "update" and "render"
// are stored. The "render" func here ultimately just gets the data the engine Renderer needs
struct GameScriptFuncData {};

class LuaScriptAPI: public Script {
  public:
    GameState gameState;
    lua_State* configScriptCtx;
    lua_State* gameScriptCtx;
    ConfigScriptData configScriptData;
    GameScriptFuncData gameFuncData;
    int loadScript(const char* filename);
    int loadConfigScript(const char* filename);
    void setConfigScriptData(int width, int height, int useFullScreen);
    void setGameScriptFunctions(void *updateFunc, void *renderFunc);
    ConfigScriptData& getConfigScriptData();
    int getInt(const char* varName);
    int getFieldInt(const char* varName);
    void callGlobalFunc(const char* funcName);
    GameState getGameState();
    ~LuaScriptAPI();
    LuaScriptAPI();
    void LuaScriptError();

};

#endif