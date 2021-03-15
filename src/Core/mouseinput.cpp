#include "mouseinput.h"

namespace ningen {

    MouseInput MouseInput::m_Instance;

    MouseInput::MouseInput(void) {}
    MouseInput::~MouseInput(void) {}

    MouseInput& MouseInput::getInstance(void)
    {
        return MouseInput::m_Instance;
    }

    void MouseInput::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) const
    {
        m_Instance.m_Xpos = xpos;
        m_Instance.m_Ypos = ypos;
    }

    void MouseInput::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) const
    {

    }

    void MouseInput::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) const
    {

    }

}