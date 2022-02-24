#ifndef KEYBOARDINPUT_H
#define KEYBOARDINPUT_H

#include "Core/core.h"
#include "GLFW/glfw3.h"
#include <functional>
#include <map>

namespace ningen {

    using Callback = std::function<void()>;
    
    class KeyboardInput
    {
        private:
            static KeyboardInput m_Instance;
            std::map<int, Callback> m_ReleaseCallbacks;
            std::map<int, Callback> m_PressCallbacks;
            KeyboardInput(void);
            ~KeyboardInput(void);
            KeyboardInput(KeyboardInput&) = delete;
            void operator=(const KeyboardInput&) = delete;
        public:
            static KeyboardInput& getInstance(void);
            void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
            void addBinding(int key, int action, const Callback& callback);
    };

}

#endif