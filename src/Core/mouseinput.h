#ifndef MOUSEINPUT_H
#define MOUSEINPUT_H

#include "Core/core.h"
#include "GLFW/glfw3.h"
#include <functional>

namespace ningen {

    class MouseInput
    {
        private:
            static MouseInput m_Instance;
            float m_Xpos, m_Ypos;
            std::function<void(float)> m_ScrollCallback;
            MouseInput(void);
            ~MouseInput(void);
            MouseInput(MouseInput&) = delete;
            void operator=(const MouseInput&) = delete;
        public:
            static MouseInput& getInstance(void);
            void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) const;
            void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) const;
            void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) const;
            void addScrollBinding(const std::function<void(float)>& callback) { m_ScrollCallback = callback; }

            inline float getX(void) const { return m_Xpos; }
            inline float getY(void) const { return m_Ypos; }
    };

}

#endif