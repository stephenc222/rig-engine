#ifndef SDL_RENDERER_H
#define SDL_RENDERER_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <map>
#include "./renderer.h"
#include "./input_manager.h"

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
    #ifdef DEBUG
    void renderDebugWindowTextLine(const char* debugText, int xOffset, int yOffset);
    void renderDebugVirtualPointerData(VirtualPointer& virtualPointer);
    void renderDebugVirtualControllersData(std::map<int, VirtualController*>& mControllers);
    void renderDebugWindow();
    #endif
    void cleanUp();
    TTF_Font* loadFont(const char *fileName, int fontSize);
    ~SDLRenderer();
    // void renderSceneGraph();
};
#endif