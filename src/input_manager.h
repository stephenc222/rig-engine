#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <map>

// assumes one per player, and just one player for now
struct VirtualController {
  void setButtonState(int buttonId, int isPressed);
  void setStickState(int stickId, float horizontal, float vertical);
  std::map<int, int> mButtonState;
  std::map<int, std::pair<float, float> > mStickState;
};

// data structure for handling mouse pointer event data
// assumes one player for now.
struct VirtualPointer {
  void setPointerDown(int isDown);
  void setPointerXY(float X, float Y);
  std::pair<float, float> pointerXY;
  int pointerDown;
};

// contains pointers to both VirtualPointer and VirtualController, for consolidated
// data passing to the Renderer for display on the debug display
struct DebugData {
  VirtualPointer* vPointerPtr;
  std::map<int, VirtualController*>* mControllersPtr;
};

// TODO: abstract later
class InputManager {
  public:
    std::map<int, VirtualController*> mControllers;
    VirtualPointer virtualPointer;
    DebugData debugData;
    void handleInput(int &isLooping);
    void handleWhichKey(SDL_Keysym *keySym, int isDown );
    void handleWhichControllerButton(int buttonId, int isPressed );
    void handleStickChange(int controllerId, int stickAxis, float axisValue);
    int getButtonState(int controllerId, int buttonId);
    std::pair<float, float> getStickState(int controllerId, int stickId);
    int getPointerDown();
    std::pair<float, float> getPointerXY();
    // for now, get all input device data
    DebugData getDebugData();

    InputManager();
    ~InputManager();
};