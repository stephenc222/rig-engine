#include <iostream>
#include "./engine.h"
#include <string>

// TODO: add a makefile rule to copy resources into bin
const char* CONFIG_FILE = "resources/scripts/config.lua";

enum ENGINE_STATUS {
  OK, 
  LOAD_TEXTURE_ERROR, 
  LOAD_CONFIG_SCRIPT_ERROR
};

int Engine::loadResources() {
  // load resources
  // resources: Textures, Scripts, GameData, etc.
  std::cout << "loading resources..." << std::endl;
  if (Engine::loadConfigScript(CONFIG_FILE) != OK) {
    return LOAD_CONFIG_SCRIPT_ERROR;
  }

  return OK;
}

int Engine::loadConfigScript(const char* filename) {
  // load config script
  // sets things like window dimensions, key mappings
  // this->luaScriptAPI;
  this->luaScriptAPI->loadConfigScript(filename);

  return OK;
}

Engine::~Engine() {
  // shutdown engine, close script contexts
  if (this->luaScriptAPI) {
    delete this->luaScriptAPI;
    this->luaScriptAPI = nullptr;
  }

};

Engine::Engine() {
  // instantiate script api class, and other api classes
  LuaScriptAPI* luaScriptAPI = new LuaScriptAPI;
  this->luaScriptAPI = luaScriptAPI;

}

int Engine::init() {
  // load files here
  int status = Engine::loadResources();
  if (status != OK) {
    Engine::engineError(status);
    return status;
  }
  Engine::start();
  return 0;
}

void Engine::start() {
  // starts the game loop
  std::cout << "start Engine \n";  
}

void Engine::engineError(int engineError) {
  std::cerr << "error initizalizing engine" << std::endl;
}