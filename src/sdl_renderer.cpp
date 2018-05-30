#include "./sdl_renderer.h"
#include "engine.h"

const char* DEBUG_FONT = "./fonts/OpenSans-Regular.ttf";

void SDLRenderer::render() {
  // render stuff
  SDL_RenderClear(this->sdlRendererPtr);

  // this->renderSceneGraph(renderer,game);
  // TODO: for now, just draw the debug window, ultimately (maybe?) wrapping this probably in
  // preprocessor directives
  this->renderDebugWindow();

  SDL_RenderPresent(this->sdlRendererPtr);
}

// you have to load a font file to then use
TTF_Font* SDLRenderer::loadFont(const char *fileName, int fontSize) {
  TTF_Font* font = TTF_OpenFont(fileName, fontSize); //this opens a font style and sets a size
  // returns a pointer to a font
  return font;
}

// TODO: add caching to reduce number of create operations
void SDLRenderer::renderDebugWindowTextLine(const char* debugText, int xOffset, int yOffset) {
  SDL_Color White = {255, 255, 255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color

  SDL_Surface* surfaceMessage = TTF_RenderText_Solid(this->debugFont, debugText, White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

  SDL_Texture* Message = SDL_CreateTextureFromSurface(this->sdlRendererPtr, surfaceMessage); //now you can convert it into a texture
  SDL_FreeSurface(surfaceMessage);


  SDL_Rect Message_rect; //create a rect
  Message_rect.x = this->debugWindowRect.x + xOffset;  //controls the rect's x coordinate 
  Message_rect.y = this->debugWindowRect.y + yOffset; // controls the rect's y coordinte
  Message_rect.w = 150; // controls the width of the rect
  Message_rect.h = 25; // controls the height of the rect

  //Mind you that (0,0) is on the top left of the window/screen, think a rect as the text's box, that way it would be very simple to understance

  //Now since it's a texture, you have to put RenderCopy in your game loop area, the area where the whole code executes

  SDL_RenderCopy(this->sdlRendererPtr, Message, NULL, &Message_rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture
  SDL_DestroyTexture(Message);
}

void SDLRenderer::renderDebugVirtualPointerData(VirtualPointer& virtualPointer) {
  // TODO: use SDLRenderer::renderDebugWindowTextLine to visually breakdown VirtualPointer
  this->renderDebugWindowTextLine("Dummy Virtual Pointer Data", 5, 5);
  
}

void SDLRenderer::renderDebugVirtualControllersData(std::map<int, VirtualController*>& mControllers) {
  // TODO: use SDLRenderer::renderDebugWindowTextLine to visually breakdown mControllers
  this->renderDebugWindowTextLine("Dummy Virtual Controllers Data", 5, 25);  
}

void SDLRenderer::renderDebugWindow() {
  // draw debug window
  
  // first, a black rectangle - for now, completely opaque
  SDL_SetRenderDrawColor(this->sdlRendererPtr, 0, 0, 0, 255);
  SDL_RenderFillRect(this->sdlRendererPtr, &this->debugWindowRect);
  // next draw text
  this->renderDebugVirtualPointerData(Engine::instance->inputManager->virtualPointer);
  this->renderDebugVirtualControllersData(Engine::instance->inputManager->mControllers);

  SDL_SetRenderDrawColor(this->sdlRendererPtr, 178, 232, 255, 255);
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
  this->debugWindowRect.x = 20;
  this->debugWindowRect.y = 20;
  this->debugWindowRect.w = 180;
  this->debugWindowRect.h = 150;

  this->debugFont = this->loadFont(DEBUG_FONT, 26);
  // TODO: maybe FIXME: for now, if no joystick (PS4 typically) is connected, assume just keyboard
  // maybe add a preprocessor directive here
  // Check for joysticks
  if( SDL_NumJoysticks() < 1 ) {
    printf( "Warning: No joysticks connected!\n" );
    this->sdlJoystick = nullptr;
  }
  else {
    //Load joystick
    SDL_JoystickEventState(SDL_ENABLE);
    this->sdlJoystick = SDL_JoystickOpen(0);
    if( this->sdlJoystick == NULL ) {
      printf( "Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError() );
      return 1;
    }
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
  // set initial background color
  SDL_SetRenderDrawColor(this->sdlRendererPtr, 178, 232, 255, 255);
  return 0;
}

void SDLRenderer::cleanUp() {
  if (this->sdlJoystick) {
    SDL_JoystickClose(this->sdlJoystick);
  }
  SDL_DestroyRenderer(this->sdlRendererPtr);
  SDL_DestroyWindow(this->sdlWindowPtr);
  IMG_Quit();
  SDL_Quit();
}

SDLRenderer::~SDLRenderer() {
  if (this->debugFont) {
    TTF_CloseFont(this->debugFont);
    this->debugFont = nullptr;
  }
};