#include "lua_script.h"
#include <iostream>
#include <stdexcept>
#include "engine.h"

int LuaScriptAPI::hello(lua_State* L) {
  std::cout << "HELLO FROM C++" << std::endl;
  return 0;
}

int LuaScriptAPI::getButtonState(lua_State* L) {
  std::map<int, bool> dummyButtonState = Engine::instance->inputManager->getButtonState();
  std::cout << "getButtonState FROM C++" << std::endl;
  // TODO: parse to lua stack friendly
  return 0;
}
int LuaScriptAPI::addOne(lua_State* L) {
  std::cout << "DO SOME ADDING!" << std::endl;
  return 1;
}

// C_API LuaScriptAPI::c_API;
int LuaScriptAPI::loadScript(const char* filename) {
  // pass the file name of the script
  std::cout << "lua game script name: " << filename << std::endl;
  if (luaL_loadfile(this->gameScriptCtx, filename) || lua_pcall(this->gameScriptCtx, 0, 0, 0)) {
    // error(L, "cannot run config. file: %s", lua_tostring(L, -1));
    std::cerr << "ERROR: can't open game script file: " << filename << std::endl;
    return 1;
  }
  luaL_openlibs(this->gameScriptCtx);
  

  // initial successful attempt at calling lua global functions from C++
  this->callGlobalFunc("update");
  this->callGlobalFunc("render");
  return 0;
}

void LuaScriptAPI::callGlobalFunc(const char* funcName) {
  // TODO: maybe FIXME: assumes the game script lua stack is empty
  // stack: []
  lua_getglobal(this->gameScriptCtx, funcName);
  // stack: [funcName]
  /* do the call (0 arguments, 1 result) */
  if (lua_isfunction(this->gameScriptCtx, -1)) {
    lua_pcall(this->gameScriptCtx, 0, 1, 0);
    // std::cout << "CALLED FUNC: " << std::string(lua_tostring(this->gameScriptCtx, -1)) << std::endl;
    lua_pop(this->gameScriptCtx, 1);
    // stack: [..]
  } else {
    lua_pop(this->gameScriptCtx, 1);
    // stack: [..]
  }
}

int LuaScriptAPI::getInt(const char* varName) {
  int isnum;
  lua_getglobal(this->configScriptCtx, varName);
  int result = (int)lua_tointegerx(this->configScriptCtx, -1, &isnum); // -1 refs value at top of lua stack
  if (!isnum) {
    std::cerr << "ERROR: Not A Number " << std::endl;
    return -1;
  }
  lua_pop(this->configScriptCtx, 1);   /* remove result from the stack */
  return result;
}

int LuaScriptAPI::loadConfigScript(const char* filename) {
  // pass the file name of the script
  std::cout << "lua script name: " << filename << std::endl;

  if (luaL_loadfile(this->configScriptCtx, filename) || lua_pcall(this->configScriptCtx, 0, 0, 0)) {
    // error(L, "cannot run config. file: %s", lua_tostring(L, -1));
    std::cerr << "ERROR: can't open config file: " << filename << std::endl;
    return 1;
  }

  // "config" is the name of a global lua table in config.lua
  lua_getglobal(this->configScriptCtx, "config");
  // stack: [config]
  int width = this->getFieldInt("WIDTH");
  int height = this->getFieldInt("HEIGHT");
  int useFullScreen = this->getFieldInt("USE_FULL_SCREEN");
  lua_pop(this->configScriptCtx, 1);
  // stack: []

  this->setConfigScriptData(width, height, useFullScreen);

  return 0;
}

int LuaScriptAPI::getFieldInt(const char* fieldName) {
  if (!lua_istable(this->configScriptCtx, -1)) {
    throw std::runtime_error("stack top is not a table");
  }
  lua_getfield(this->configScriptCtx, -1, fieldName);
  // stack: [table, fieldName]
  if (!lua_isnumber(this->configScriptCtx, -1)) {
    // TODO: decide if a default would be better here
    throw std::runtime_error("Config fieldName is not a number");
  }
  int fieldValue = (int)lua_tointegerx(this->configScriptCtx, -1, 0);

  lua_pop(this->configScriptCtx, 1);
  return fieldValue;
}

GameState LuaScriptAPI::getGameState() {
  // get the current game state
  // get gameState from lua context

  return this->gameState;
};

void LuaScriptAPI::setConfigScriptData(int width, int height, int useFullScreen) {
  this->configScriptData.width = width;
  this->configScriptData.height = height;
  this->configScriptData.useFullScreen = useFullScreen;
}

ConfigScriptData& LuaScriptAPI::getConfigScriptData() {
  return this->configScriptData;
};

LuaScriptAPI::~LuaScriptAPI() {
  // close script contexts
  if (this->configScriptCtx) {
    lua_close(this->configScriptCtx);
    this->configScriptCtx = nullptr;
  }
  if (this->gameScriptCtx) {
    lua_close(this->gameScriptCtx);
    this->gameScriptCtx = nullptr;
  }
}

LuaScriptAPI::LuaScriptAPI() { 
  this->configScriptCtx = luaL_newstate();
  this->gameScriptCtx = luaL_newstate();
  // luaL_openlibs(this->gameScriptCtx);
  // library to be registered
  static const struct luaL_Reg api[] = {
    { "hello", LuaScriptAPI::hello },
    { "getButtonState", LuaScriptAPI::getButtonState },
    { "addOne", LuaScriptAPI::addOne },
    { nullptr, nullptr }
  };

  luaL_newlib(this->gameScriptCtx, api);
  lua_setglobal(this->gameScriptCtx, "api");
}