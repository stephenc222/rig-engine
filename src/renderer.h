#ifndef Renderer_H
#define Renderer_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class Renderer {
  public:
    virtual void render() = 0;
    virtual double getTime() = 0;
    virtual int init(int SCREEN_WIDTH, int SCREEN_HEIGHT) = 0;
    virtual void cleanUp() = 0;
    Renderer() {};
    virtual ~Renderer() {};
    // void renderSceneGraph();
};
#endif