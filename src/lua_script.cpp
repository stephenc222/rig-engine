#include "lua_script.h"
#include <iostream>
#include <stdexcept>
#include <map>
#include "engine.h"

// NOTE: PS4 constants are hardcoded and possibly non-standard for SDL
static const char* scancodeGlobals[] = {
  "PS4_SQUARE",
  "PS4_X",
  "PS4_CIRCLE",
  "PS4_TRIANGLE",
  "PS4_LEFT_BUMPER",
  "PS4_RIGHT_BUMPER",
  "PS4_LEFT_TRIGGER",
  "PS4_RIGHT_TRIGGER",
  "PS4_SHARE_BUTTON",
  "PS4_OPTION_BUTTON",
  "PS4_LEFT_STICK_BUTTON",
  "PS4_RIGHT_STICK_BUTTON",
  "PS4_LOGO_BUTTON",
  "PS4_TOUCHPAD_BUTTON",
  // standard scan codes from here
  "SDL_SCANCODE_A",
  "SDL_SCANCODE_LEFT",
  "SDL_SCANCODE_D",
  "SDL_SCANCODE_RIGHT",
  "SDL_SCANCODE_W",
  "SDL_SCANCODE_UP",
  "SDL_SCANCODE_S",
  "SDL_SCANCODE_DOWN",
  "SDL_SCANCODE_RETURN",
  "SDL_QUIT",
  "SDL_KEYDOWN",
  "SDL_KEYUP",
  "SDL_MOUSEBUTTONDOWN",
  "SDL_MOUSEBUTTONUP",
  "SDL_MOUSEMOTION",
  "SDL_CONTROLLERBUTTONDOWN",
  "SDL_CONTROLLERBUTTONUP",
  "SDL_CONTROLLERAXISMOTION",
  "SDL_JOYAXISMOTION",
  "SDL_JOYBUTTONDOWN",
  "SDL_JOYBUTTONUP",
};

// TODO: hardcoded + 500 to the default values of PS4 button indices
// @see InputManager::handleInput
static const int scancodeValues[] = {
  500,
  501,
  502,
  503,
  504,
  505,
  506,
  507,
  508,
  509,
  510,
  511,
  512,
  513,
  // standard scancode values from here
  SDL_SCANCODE_A,
  SDL_SCANCODE_LEFT,
  SDL_SCANCODE_D,
  SDL_SCANCODE_RIGHT,
  SDL_SCANCODE_W,
  SDL_SCANCODE_UP,
  SDL_SCANCODE_S,
  SDL_SCANCODE_DOWN,
  SDL_SCANCODE_RETURN,
  SDL_QUIT,
  SDL_KEYDOWN,
  SDL_KEYUP,
  SDL_MOUSEBUTTONDOWN,
  SDL_MOUSEBUTTONUP,
  SDL_MOUSEMOTION,
  SDL_CONTROLLERBUTTONDOWN,
  SDL_CONTROLLERBUTTONUP,
  SDL_CONTROLLERAXISMOTION,
  SDL_JOYAXISMOTION,
  SDL_JOYBUTTONDOWN,
  SDL_JOYBUTTONUP,
};


void LuaScriptAPI::setScriptGlobals(lua_State* L) {
  // register constant values of particular keys and buttons as globals in the script context
  int numCodes = sizeof(scancodeGlobals) / sizeof(const char*);
  for (int i = 0; i < numCodes; i += 1) {
    lua_pushnumber(L, scancodeValues[i]);
    lua_setglobal(L, scancodeGlobals[i]);
  }
}
/**
 * Returns to the calling Lua script the state of a controller (or keyboard) button/key
 * @param L a pointer to a lua context
 * @return the number of results the lua function can expect
*/
int LuaScriptAPI::getButtonState(lua_State* L) {
  // TODO: get these Id's from the lua script
  // NOTE: -1 means top of stack
  int controllerId;
  int buttonId; // also stands for keyId for keyboard input
  int isDown;
  // for now controllerId is a consistent 0
  controllerId = 0;
  buttonId = lua_tointeger(L, -1); // get argument
  lua_pop(L, -1);
  isDown = Engine::instance->inputManager->getButtonState(controllerId, buttonId);
  // this button state is returned to the calling lua function
  lua_pushnumber(L, isDown);
  return 1;
}

int LuaScriptAPI::getStickState(lua_State* L) {
  // std::cout << "getStickState FROM C++" << std::endl;
  int controllerId;
  int stickId;
  float horizontal;
  float vertical;

  // for now controllerId is a consistent 0
  controllerId = 0;
  stickId = lua_tointeger(L, -1);
  lua_pop(L, -1);
  std::pair<float, float> stickState = Engine::instance->inputManager->getStickState(controllerId, stickId);
  horizontal = stickState.first;
  vertical = stickState.second;
  // push horizontal and vertical values on the stack
  lua_pushnumber(L, horizontal);
  lua_pushnumber(L, vertical);
  
  return 2;
}

int LuaScriptAPI::getPointerDown(lua_State* L) {
    // no arguments expected since only one mouse pointer is expected
  // std::cout << "getPointerDown FROM C++" << std::endl;
  int isDown;
  isDown = Engine::instance->inputManager->getPointerDown();
  lua_pushnumber(L, isDown);
  return 1;
}

int LuaScriptAPI::getPointerXY(lua_State* L) {
  // std::cout << "getPointerXY FROM C++" << std::endl;
  std::pair<float, float> pointerXY;
  float pointerX;
  float pointerY;

  pointerXY = Engine::instance->inputManager->getPointerXY();
  // no arguments expected since only one mouse pointer is expected
  pointerX = pointerXY.first;
  pointerY = pointerXY.second;
  lua_pushnumber(L, pointerX);
  lua_pushnumber(L, pointerY);
  return 2;
}

int LuaScriptAPI::loadScript(const char* filename) {
  // pass the file name of the script
  std::cout << "lua game script name: " << filename << std::endl;
  luaL_openlibs(this->gameScriptCtx);
  this->setScriptGlobals(this->gameScriptCtx);

  if (luaL_loadfile(this->gameScriptCtx, filename) || lua_pcall(this->gameScriptCtx, 0, 0, 0)) {
    // error(L, "cannot run config. file: %s", lua_tostring(L, -1));
    std::cerr << "ERROR: can't open game script file: " << filename << std::endl;
    return 1;
  }

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
    { "getButtonState", LuaScriptAPI::getButtonState },
    { "getStickState", LuaScriptAPI::getStickState},
    { "getPointerDown", LuaScriptAPI::getPointerDown },
    { "getPointerXY", LuaScriptAPI::getPointerXY},
    { nullptr, nullptr }
  };

  luaL_newlib(this->gameScriptCtx, api);
  lua_setglobal(this->gameScriptCtx, "api");
}