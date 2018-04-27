#include <iostream>
#include "./engine.h"


enum ENGINE_STATUS {
  OK, 
  LOAD_TEXTURE_ERROR, 
  LOAD_CONFIG_SCRIPT_ERROR
};

int Engine::loadResources() {
  // load resources
  // resources: Textures, Scripts, GameData, etc.
  std::cout << "loading resources..." << std::endl;
  if (Engine::loadConfigScript() != OK) {
    return LOAD_CONFIG_SCRIPT_ERROR;
  }

  return OK;
}

int Engine::loadConfigScript() {
  // load config script
  // sets things like window dimensions, key mappings
  this->configScriptCtx = luaL_newstate();

  return OK;
}

Engine::~Engine() {
  // shutdown engine, close script contexts
  lua_close(this->configScriptCtx);

};

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