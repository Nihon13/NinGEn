#ifndef WINDOW_H
#define WINDOW_H

#include "GLFW/glfw3.h"
#include "Core/core.h"

namespace ningen {

    class Window
    {
        private:
            GLFWwindow* m_Window;
            const char* m_Title;
            int m_Width, m_Height;

            // bool createWindow(void);
            // void closeWindow(void) const;
        public:
            Window(const char* title = "NinGEn - Keep it simple!", int width = 1600, int height = 900);
            ~Window(void);
    };

}

#endif