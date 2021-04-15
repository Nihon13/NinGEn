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
        if (action != GLFW_RELEASE && key == GLFW_KEY_ESCAPE) // TODO: To change
        {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        if (action == GLFW_PRESS && key == GLFW_KEY_F1)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        if (action == GLFW_RELEASE && key == GLFW_KEY_F1)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }

}