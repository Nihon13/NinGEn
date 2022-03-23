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

        float offsetX = m_Xpos - m_LastXpos;
        float offsetY = m_LastYpos - m_Ypos;

        m_Instance.m_LastXpos = xpos;
        m_Instance.m_LastYpos = ypos;

        if (m_IsRightButtonPressed)
        {
            m_RightButtonCallback(offsetX);
        }
    }

    void MouseInput::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) const
    {
        if (m_ScrollCallback != NULL)
        {
            m_ScrollCallback(yoffset);
        }
    }

    void MouseInput::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) const
    {
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            m_Instance.m_IsRightButtonPressed = true;

            if (m_IsRightButtonPressed)
            {
                m_Instance.m_LastXpos = m_Xpos;
                m_Instance.m_LastYpos = m_Ypos;
            }
        }

        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            m_Instance.m_IsRightButtonPressed = false;
        }
    }

}