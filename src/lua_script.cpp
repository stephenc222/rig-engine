#include "lua_script.h"
#include <iostream>
#include <string>



int LuaScriptAPI::loadScript(std::string& filename) {
  // pass the file name of the script
  return 0;
}

int LuaScriptAPI::getInt(std::string& varName) {
  int isnum;
  lua_getglobal(this->configScriptCtx, varName.c_str());
  int result = (int)lua_tointegerx(this->configScriptCtx, -1, &isnum); // -1 refs value at top of lua stack
  if (!isnum) {
    std::cerr << "ERROR: Not A Number " << std::endl;
    return -1;
  }
  lua_pop(this->configScriptCtx, 1);   /* remove result from the stack */
  return result;
}

int LuaScriptAPI::loadConfigScript(std::string& filename) {
  // pass the file name of the script
  std::cout << "lua script name: " << filename << std::endl;

  if (luaL_loadfile(this->configScriptCtx, filename.c_str()) || lua_pcall(this->configScriptCtx, 0, 0, 0)) {
    // error(L, "cannot run config. file: %s", lua_tostring(L, -1));
    std::cerr << "ERROR: can't open config file: " << filename << std::endl;
    return 1;
  }

  // TODO: WIP handling lua variables
  std::string widthVar("WIDTH");
  std::string heightVar("HEIGHT");
  int w = this->getInt(widthVar);
  int h = this->getInt(heightVar);
  std::cout << "W: " << w << " H: " << h << std::endl;

  return 0;
}

GameState LuaScriptAPI::getGameState() {
  // get the current game state
  // get gameState from lua context

  return this->gameState;
}

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