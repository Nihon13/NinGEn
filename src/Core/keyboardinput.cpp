#include "keyboardinput.h"

namespace ningen {

    KeyboardInput KeyboardInput::m_Instance;

    KeyboardInput::KeyboardInput(void) {}
    KeyboardInput::~KeyboardInput(void) {}

    KeyboardInput& KeyboardInput::getInstance(void)
    {
        return KeyboardInput::m_Instance;
    }

    void KeyboardInput::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) const
    {
        
    }

}