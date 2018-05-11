// some_header_file.h
#ifndef ENGINE_H
#define ENGINE_H

#include "./lua_script.h"
#include "./input_manager.h"
#include "./renderer.h"


struct ConfigData {
  int screenWidth;
  int screenHeight;
  int useFullScreen;
};

class Engine {
  private:
    ConfigData configData;
    LuaScriptAPI* luaScriptAPI;
    Renderer* renderer;
    InputManager* inputManager;
    int isLooping;
  public:
    int init();
    // start engine
    void start();
    // load resources 
    int loadResources();
    // handle input
    void setConfigData(ConfigScriptData& configScriptData);
    ConfigData& getConfigData();
    int loadConfigScript(const char* filename);
    Engine();
    ~Engine();
    // reports engine errors
    void engineError(int engineError);
    void update(float dt);
    // void cleanUp
};

#endif