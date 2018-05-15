// some_header_file.h
#ifndef ENGINE_H
#define ENGINE_H

#include "./lua_script.h"
#include "./input_manager.h"
#include "sdl_renderer.h"

struct ConfigData {
  int screenWidth;
  int screenHeight;
  int useFullScreen;
};

struct GameFuncs {};

class Engine {
  public:
    ConfigData configData;
    GameFuncs gameFuncs;
    LuaScriptAPI* luaScriptAPI;
    Renderer* renderer;
    InputManager* inputManager;
    static Engine* instance;
    int isLooping;
    int init();
    // start engine
    void start();
    // load resources 
    int loadResources();
    // handle input
    void setConfigData(ConfigScriptData& configScriptData);
    void setGameScriptFunctions(GameScriptFuncData& gameScriptFuncData);
    ConfigData& getConfigData();
    int loadConfigScript(const char* filename);
    int loadScript(const char* filename);
    Engine();
    ~Engine();
    // reports engine errors
    void handleInput(int *isLooping);
    void engineError(int engineError);
    void update(float dt);
    void render();
    // void cleanUp
};

#endif