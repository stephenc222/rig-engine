#include "./sdl_renderer.h"

void SDLRenderer::render() {
  // render stuff
  SDL_RenderClear(this->sdlRendererPtr);

  // this->renderSceneGraph(renderer,game);

  SDL_RenderPresent(this->sdlRendererPtr);
}

double SDLRenderer::getTime() {
  return SDL_GetTicks();
}

int SDLRenderer::init(int SCREEN_WIDTH, int SCREEN_HEIGHT) {
  printf("init SDLRenderer!\n");

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("SDL_INIT Error: %s \n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  if(TTF_Init() != 0) {
    printf("TTF_Init: %s\n", TTF_GetError());
    return 1;
  }
  
  this->sdlWindowPtr = SDL_CreateWindow(
    "Rig Engine Demo", 
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, 
    SCREEN_WIDTH, 
    SCREEN_HEIGHT, 
    SDL_WINDOW_RESIZABLE
  );

  if (this->sdlWindowPtr == NULL) {
    printf("SDL_CreateWindow Error: %s \n", SDL_GetError());
	  SDL_Quit();
	  return 1;
  }

  this->sdlRendererPtr = SDL_CreateRenderer(this->sdlWindowPtr, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (this->sdlRendererPtr == NULL) {
    SDL_DestroyWindow(this->sdlWindowPtr);
    printf("SDL_CreateRenderer Error: %s \n", SDL_GetError());
    SDL_Quit();
    return 1;
  } 

  if (SDL_RenderSetLogicalSize(this->sdlRendererPtr, SCREEN_WIDTH, SCREEN_HEIGHT) != 0) {
    SDL_DestroyWindow(this->sdlWindowPtr);
    printf("SDL_RenderSetLogicalSize Error: %s \n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  // for allowing loading of PNG files. IMG_INIT_PNG == 2
  if(IMG_Init(IMG_INIT_PNG) != 2){
    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
    return 1;
  }   
  
  SDL_SetRenderDrawColor(this->sdlRendererPtr, 178, 232, 255, 255);
  return 0;
}

void SDLRenderer::cleanUp() {
  SDL_DestroyRenderer(this->sdlRendererPtr);
  SDL_DestroyWindow(this->sdlWindowPtr);
  IMG_Quit();
  SDL_Quit();
}

SDLRenderer::~SDLRenderer() {};