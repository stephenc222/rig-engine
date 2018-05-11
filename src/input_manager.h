#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// TODO: abstract later
class InputManager {
  public:
    void handleInput(int &isLooping);
};