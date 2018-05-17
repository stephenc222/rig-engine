#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "./renderer.h"

class SDLRenderer: public Renderer {
  public:
    SDL_Renderer* sdlRendererPtr;
    SDL_Window* sdlWindowPtr;
    SDL_Joystick* sdlJoystick;
    void render();
    double getTime();
    int init(int SCREEN_WIDTH, int SCREEN_HEIGHT);
    void cleanUp();
    ~SDLRenderer();
    // void renderSceneGraph();
};