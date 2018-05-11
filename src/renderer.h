#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class Renderer {
  public:
    SDL_Renderer* sdlRendererPtr;
    SDL_Window* sdlWindowPtr;
    void render();
    double getTime();
    int init(int SCREEN_WIDTH, int SCREEN_HEIGHT);
    void cleanUp();
    // void renderSceneGraph();
};