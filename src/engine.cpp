#include <iostream>
#include "./engine.h"
#include <string>
Engine* Engine::instance = nullptr;

// TODO: replace this with a dynamic string getter
const char* CONFIG_FILE = "./scripts/config.lua";
const char* GAME_FILE = "./scripts/game.lua";

enum ENGINE_STATUS {
  OK, 
  LOAD_TEXTURE_ERROR, 
  LOAD_CONFIG_SCRIPT_ERROR,
  LOAD_SCRIPT_ERROR
};

int Engine::loadResources() {
  // load resources
  // resources: Textures, Scripts, GameData, etc.
  std::cout << "loading resources..." << std::endl;
  if (Engine::loadConfigScript(CONFIG_FILE) != OK) {
    return LOAD_CONFIG_SCRIPT_ERROR;
  }
  if (Engine::loadScript(GAME_FILE) != OK) {
    return LOAD_SCRIPT_ERROR;
  }

  return OK;
}

int Engine::loadConfigScript(const char* filename) {
  // load config script
  // sets things like window dimensions, key mappings
  // this->luaScriptAPI;
  this->luaScriptAPI->loadConfigScript(filename);
  // this->configData = this->luaScriptAPI->configScriptData;
  this->setConfigData(this->luaScriptAPI->configScriptData);
  return OK; 
}

void Engine::setGameScriptFunctions(GameScriptFuncData& gameScriptFuncData) {
  // set the main game script functions of "update" and "render" to the Engine
}

int Engine::loadScript(const char* filename) {
  // load config script
  // sets things like window dimensions, key mappings
  // this->luaScriptAPI;
  this->luaScriptAPI->loadScript(filename);
  // this->configData = this->luaScriptAPI->configScriptData;
  this->setGameScriptFunctions(this->luaScriptAPI->gameFuncData);
  return OK;
}

ConfigData& Engine::getConfigData() {
  return this->configData;
};

void Engine::setConfigData(ConfigScriptData& configScriptData) {
  if (configScriptData.width != 0) {
    this->configData.screenWidth = configScriptData.width;
  }
  if (configScriptData.height != 0) {
    this->configData.screenHeight = configScriptData.height;
  }
  if (configScriptData.useFullScreen != 0) {
    this->configData.useFullScreen = configScriptData.useFullScreen;
  } else {
    this->configData.useFullScreen = 0;
  }
};

Engine::~Engine() {
  // shutdown engine, close script contexts
  if (this->luaScriptAPI) {
    delete this->luaScriptAPI;
    this->luaScriptAPI = nullptr;
  }
  if (this->inputManager) {
    delete this->inputManager;
    this->inputManager = nullptr;
  }
  if (this->renderer) {
    #ifdef SDL
      delete this->renderer;
    #endif
    this->renderer = nullptr;
  }

};

Engine::Engine() {
  // instantiate script api class, and other api classes
  ConfigData configData;
  Engine::instance = this;
  this->configData = configData;
  this->luaScriptAPI = new LuaScriptAPI;
  // only one renderer kind per build
  #ifdef SDL
    this->renderer = new SDLRenderer;
  #endif
  this->inputManager = new InputManager;
  this->isLooping = 1;

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

void Engine::handleInput(int *isLooping) {
  // handleInput will set isLooping to 0, killing the game loop if the quit button is pressed
  // TODO: enable game to have "cleanup" or "abrupt exit handling" if isLooping is 0
  this->inputManager->handleInput(*isLooping);

}

void Engine::update(float dt) {
  // TODO: handle game update logic through script function here
  this->luaScriptAPI->callGlobalFunc("update");
}

void Engine::render() {
  // TODO: pass render data to renderer from script's relevant render data
  this->luaScriptAPI->callGlobalFunc("render");
  this->renderer->render();
}

void Engine::start() {
  // starts the game loop
  std::cout << "start Engine \n";  
  this->renderer->init(this->configData.screenWidth, this->configData.screenHeight);
  float dt = 0;
  double currentTime, lastTime = 0;
  while(this->isLooping) {
    this->handleInput(&this->isLooping);
    // currentTime = SDL_GetTicks();
    currentTime = this->renderer->getTime();
    if ((currentTime - lastTime) < 1000) {
      dt = (currentTime - lastTime);
      this->update(dt);
    }
    lastTime = currentTime;
    // get data from scripting game state that's required for rendering
    this->render();
  }
  this->renderer->cleanUp();
  // cleanUp(this->renderer);
}

void Engine::engineError(int engineError) {
  std::cerr << "error initizalizing engine" << std::endl;
}