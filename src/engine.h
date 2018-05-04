// some_header_file.h
#ifndef ENGINE_H
#define ENGINE_H

#include "./lua_script.h"
#include <string>


struct ConfigData {
  int screenWidth;
  int screenHeight;
  int useFullScreen;
};

class Engine {
  private:
    ConfigData configData;
    LuaScriptAPI* luaScriptAPI;
  public:
    int init();
    // start engine
    void start();
    // load resources
    int loadResources();
    int loadConfigScript(std::string& filename);
    Engine();
    ~Engine();
    // reports engine errors
    void engineError(int engineError);
};

#endif