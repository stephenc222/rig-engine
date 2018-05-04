// Script Interface Class

// implemented for handling a variety of scripting languages
// provides an interface to the scripting language to control various parts of the engine

#ifndef SCRIPT_H
#define SCRIPT_H
#include <string>

struct GameState {};

class Script {
  public:
    GameState *gameState;
    // used on start up
    virtual int loadScript(std::string& filename) = 0;
    // used either before at the start of the render call
    virtual GameState getGameState() = 0;
};

#endif