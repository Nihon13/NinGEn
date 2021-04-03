#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_INCLUDE_NONE
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Core/core.h"
#include "Core/keyboardinput.h"
#include "Core/mouseinput.h"

namespace ningen {

    class Window
    {
        private:
            GLFWwindow* m_Window;
            const char* m_Title;
            int m_Width, m_Height;

            bool createWindow(void);
            void closeWindow(void) const;
            static void error_callback(int code, const char* description);
            static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
            static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
            static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
            static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
            static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
            static void opengl_error_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
        public:
            Window(const char* title = "NinGEn - Keep it simple!", int width = 1200, int height = 675);
            ~Window(void);
            void clearWindow(void) const;
            void updateWindow(void) const;
            bool windowShouldClose(void) const;
    };

}

#endif