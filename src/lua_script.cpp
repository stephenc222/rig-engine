#include "lua_script.h"
#include <iostream>
#include <stdexcept>

int LuaScriptAPI::loadScript(const char* filename) {
  // pass the file name of the script
  return 0;
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
}

LuaScriptAPI::LuaScriptAPI() { 
  this->configScriptCtx = luaL_newstate();
}