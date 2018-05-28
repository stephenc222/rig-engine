#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "./renderer.h"

class SDLRenderer: public Renderer {
  public:
    SDL_Renderer* sdlRendererPtr;
    SDL_Window* sdlWindowPtr;
    SDL_Joystick* sdlJoystick;
    SDL_Rect debugWindowRect;
    TTF_Font* debugFont;
    void render();
    double getTime();
    int init(int SCREEN_WIDTH, int SCREEN_HEIGHT);
    void renderDebugWindowTextLine(const char* debugText);
    void renderDebugWindow();
    void cleanUp();
    TTF_Font* loadFont(const char *fileName, int fontSize);
    ~SDLRenderer();
    // void renderSceneGraph();
};