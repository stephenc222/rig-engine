#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <map>

struct VirtualController {
  void setButtonState(int buttonId, bool isPressed);
  void setStickState(int stickId, float horizontal, float vertical);
  std::map<int, bool> mButtonState;
  std::map<int, std::pair<float, float> > mStickState;
};

// TODO: abstract later
class InputManager {
  public:
    int dummyValue;
    std::map<int, VirtualController*> mControllers;
    void handleInput(int &isLooping);
    std::map<int, bool> getButtonState();
    // TODO: WIP trying out static pointer from Engine
};