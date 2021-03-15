#ifndef KEYBOARDINPUT_H
#define KEYBOARDINPUT_H

#include "Core/core.h"
#include "GLFW/glfw3.h"

namespace ningen {

    class KeyboardInput
    {
        private:
            static KeyboardInput m_Instance;
            KeyboardInput(void);
            ~KeyboardInput(void);
            KeyboardInput(KeyboardInput&) = delete;
            void operator=(const KeyboardInput&) = delete;
        public:
            static KeyboardInput& getInstance(void);
            void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) const;
    };

}

#endif