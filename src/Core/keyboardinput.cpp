#include "keyboardinput.h"

namespace ningen {

    KeyboardInput KeyboardInput::m_Instance;

    KeyboardInput::KeyboardInput(void) {}
    KeyboardInput::~KeyboardInput(void) {}

    KeyboardInput& KeyboardInput::getInstance(void)
    {
        return KeyboardInput::m_Instance;
    }

    void KeyboardInput::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (action == GLFW_RELEASE)
        {
            if (m_ReleaseCallbacks[key] != NULL)
            {
                m_ReleaseCallbacks[key]();
            }
        }
        else
        {
            if (m_PressCallbacks[key] != NULL)
            {
                m_PressCallbacks[key]();
            }
        }
    }

    void KeyboardInput::addBinding(int key, int action, const Callback& callback)
    {
        if (action == GLFW_RELEASE)
        {
            m_ReleaseCallbacks[key] = callback;
        }
        else
        {
            m_PressCallbacks[key] = callback;
        }
    }
    
}