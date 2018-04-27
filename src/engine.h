#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"

struct ConfigData {
  int screenWidth;
  int screenHeight;
  int useFullScreen;
};

class Engine {
  private:
    ConfigData configData;
    lua_State* configScriptCtx;
  public:
    int init();
    // start engine
    void start();
    // load resources
    int loadResources();
    int loadConfigScript();
    // Engine();
    ~Engine();
    // reports engine errors
    void engineError(int engineError);
};
