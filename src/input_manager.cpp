#include "./input_manager.h"
#include <iostream>

void InputManager::handleInput(int& isLooping) {
  SDL_Event event;
  while(SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT: {
        isLooping = 0;        
      }
    }
  }
}

std::map<int, bool> InputManager::getButtonState() {
  // TODO: WIP, connect to VirtualController collection
  std::map<int, bool> dummyButtonState;
  dummyButtonState[0] = 26;
  std::cout << "input manager method called NOW" << std::endl;
  return dummyButtonState;
}
