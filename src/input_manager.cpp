#include "./input_manager.h"
#include <iostream>

// TODO: later - an optimization could occur for removing the possibility of certain event types based
// on platform the engine is built for
void InputManager::handleWhichControllerButton(int buttonId, int isPressed) {
  /* For now, assumes only one controller at index 0
    On the PS4 Controller - buttonId:

    Square          ==  0
    X               ==  1
    Circle          ==  2
    Triangle        ==  3
    L. Bumper       ==  4
    R. Bumper       ==  5
    L. Trigger      ==  6
    R. Trigger      ==  7
    Share Button    ==  8
    Option Button   ==  9
    L. Stick Press  == 10
    R. Stick Press  == 11
    PS Button       == 12
    Touhpad Press   == 13
  */
  this->mControllers[0]->setButtonState(buttonId, isPressed);
}

void InputManager::handleWhichKey(SDL_Keysym *keySym, int isDown ) {
  /* 
    For now, assumes only one controller at index 0
    NOTE: controller buttons and keyboard keys down and up events are handled the same way
    via buttonState
    
    Typically:

    W == UP
    A == LEFT
    S == DOWN
    D == RIGHT

    but this is going to be relegated to the game specific logic layer
  */

  switch (keySym->scancode) {
    case SDL_SCANCODE_A: {
      this->mControllers[0]->setButtonState(SDL_SCANCODE_A, isDown);
      break;
    }
    case SDL_SCANCODE_LEFT: {
      this->mControllers[0]->setButtonState(SDL_SCANCODE_LEFT, isDown);
      break;
    }
    case SDL_SCANCODE_D: {
      this->mControllers[0]->setButtonState(SDL_SCANCODE_D, isDown);
      break;
    }
    case SDL_SCANCODE_RIGHT: {
      this->mControllers[0]->setButtonState(SDL_SCANCODE_RIGHT, isDown);
      break;
    }
    case SDL_SCANCODE_W: { 
      this->mControllers[0]->setButtonState(SDL_SCANCODE_W, isDown);
      break;
    }
    case SDL_SCANCODE_UP: {
      this->mControllers[0]->setButtonState(SDL_SCANCODE_UP, isDown);
      break;
    }
    case SDL_SCANCODE_S: {
      this->mControllers[0]->setButtonState(SDL_SCANCODE_S, isDown);
      break;
    }
    case SDL_SCANCODE_DOWN: {
      this->mControllers[0]->setButtonState(SDL_SCANCODE_DOWN, isDown);
      break;
    }
    case SDL_SCANCODE_RETURN: {
      this->mControllers[0]->setButtonState(SDL_SCANCODE_RETURN, isDown);
      break;
    }
    default: {
      break;
    }
  }
}

void InputManager::handleInput(int& isLooping) {
  SDL_Event event;
  while(SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT: {
        isLooping = 0;        
      }
      case SDL_KEYDOWN: {
        int isDown = 1;
        this->handleWhichKey(&event.key.keysym, isDown );
        break;
      }
      case SDL_KEYUP: {
        int isDown = 0;
        this->handleWhichKey(&event.key.keysym, isDown );
        break;
      }
      case SDL_MOUSEBUTTONDOWN: {
        int isDown = 1;       
        this->virtualPointer.setPointerDown(isDown);                 
        break;
      }
      case SDL_MOUSEBUTTONUP: {
        int isDown = 0;       
        this->virtualPointer.setPointerDown(isDown);                 
        break;
      }
      case SDL_MOUSEMOTION: {
        this->virtualPointer.setPointerXY(static_cast<float>(event.motion.x), static_cast<float>(event.motion.y));
        break;
      }
      case SDL_CONTROLLERBUTTONDOWN: {
        // which controller button
        // event.button
        break;
      }
      case SDL_CONTROLLERBUTTONUP: {
        break;
      }
      case SDL_CONTROLLERAXISMOTION: {
        break;
      }
      // applies to joy stick events
      case SDL_JOYAXISMOTION: {
        // TODO: implement reducing sensitivity of control stick
        // TODO: figure out why L. Trigger and R. Trigger "halfway presses" triggers this event
        // std::cout << "moved the stick!" << std::endl;
        break;
      }
      case SDL_JOYBUTTONDOWN: {
        // SDL_Log("Joystick %d button %d down\n", event.jbutton.which, event.jbutton.button);
        // TODO: find a better way to reuse the VirtualController data structure 
        // while separating buttons from keys than just adding 500?
        int isPressed = 1;
        this->handleWhichControllerButton(static_cast<int>(event.jbutton.button + 500), isPressed);
        break;
      }
      case SDL_JOYBUTTONUP: {
        // SDL_Log("Joystick %d button %d up\n", event.jbutton.which, event.jbutton.button);
        int isPressed = 0;
        this->handleWhichControllerButton(static_cast<int>(event.jbutton.button + 500), isPressed);
        break;
      }
      case SDL_JOYDEVICEADDED: {
        std::cout << "joy stick connected!" << std::endl;
        break;
      }
      case SDL_JOYDEVICEREMOVED: {
        std::cout << "joy stick disconnected!" << std::endl;
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
  std::cout << "calling getButtonState! buttonId:" << buttonId << std::endl;
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