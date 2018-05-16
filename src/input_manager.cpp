#include "./input_manager.h"
#include <iostream>

void InputManager::handleInput(int& isLooping) {
  SDL_Event event;
  while(SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT: {
        isLooping = 0;        
      }
      case SDL_KEYDOWN: {
        // printf("keyDown and: %u\n", event.key.keysym);
        // if (game->gameInput->isKeyDown == 1) {
        //   break;
        // }
        // game->gameInput->isKeyDown = 1;
        // handleWhichKey(game, &event.key.keysym);
        break;
      }
      case SDL_KEYUP: {
        // printf("keyUp!\n");
        // game->gameInput->isKeyDown = 0;     
        // game->gameInput->whichKey = 0;   
        break;
      }
      case SDL_MOUSEBUTTONDOWN: {
        // printf("mouseDown!\n");
        // game->gameInput->isMouseDown = 1;    
        // handleMouseDown(renderer,game);
        int isDown = 1;       
        this->virtualPointer.setPointerDown(isDown);                 
        break;
      }
      case SDL_MOUSEBUTTONUP: {
        // printf("mouseUp!\n");
        // game->gameInput->isMouseDown = 0;    
        int isDown = 0;       
        this->virtualPointer.setPointerDown(isDown);                 
        break;
      }
      case SDL_MOUSEMOTION: {
        // printf("X: %d, Y: %d \n", event.motion.x, event.motion.y);
        // game->gameInput->mouseX = event.motion.x;
        // game->gameInput->mouseY = event.motion.y;
        break;
      }
      default: {
        break;
      }
    }
  }
}

void VirtualController::setButtonState(int buttonId, int isPressed ) {
  this->mButtonState[buttonId] = isPressed;
}

int InputManager::getButtonState(int controllerId, int buttonId) {
  // TODO: add exception handling here in case controllerIds or buttonIds don't exist
  return this->mControllers[controllerId]->mButtonState[buttonId];
}

void VirtualController::setStickState(int stickId, float horizontal, float vertical) {
  this->mStickState[stickId].first = horizontal;
  this->mStickState[stickId].second = vertical;
}

std::pair<float, float> InputManager::getStickState(int controllerId, int stickId) {
  return  this->mControllers[controllerId]->mStickState[stickId];
};

std::pair<float, float> InputManager::getPointerXY() {
  return this->virtualPointer.pointerXY;
}

void VirtualPointer::setPointerXY(float x, float y) {
  this->pointerXY.first = x;
  this->pointerXY.second = y;
}

int InputManager::getPointerDown() {
  return this->virtualPointer.pointerDown;
}

void VirtualPointer::setPointerDown(int isDown) {
  // std::cout << "set pointer down called!" << std::endl;
  this->pointerDown = isDown;
  // std::cout << "VALUE: " << this->pointerDown << std::endl;
}

InputManager::InputManager() {
  // TODO: assumes only 1 controller so 1 player for now
  this->mControllers[0] = new VirtualController;
}

InputManager::~InputManager() {
  // TODO: assumes only 1 controller so 1 player for now
  if (this->mControllers[0]) {
    delete this->mControllers[0];
    this->mControllers[0] = nullptr;
  }
}