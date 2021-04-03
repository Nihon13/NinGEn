#include "window.h"

namespace ningen {

    Window::Window(const char* title, int width, int height)
        : m_Title(title), m_Width(width), m_Height(height)
    {
        if (!createWindow())
        {
            LOG_ERROR("Failed to create window!");
        }
    }

    Window::~Window(void)
    {
        closeWindow();
    }

    bool Window::createWindow(void)
    {
        glfwSetErrorCallback(error_callback);

        if (!glfwInit())
        {
            LOG_ERROR("Failed to initialize GLFW!");
            return false;
        }

        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        if (m_Width > mode->width) m_Width = mode->width;
        if (m_Height > mode->height) m_Height = mode->height;

        m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, NULL, NULL);

        if (!m_Window)
        {
            LOG_ERROR("Failed to create window!");
            glfwTerminate();
            return false;
        }

        glfwSetWindowPos(m_Window, ((mode->width) - m_Width)/2, ((mode->height) - m_Height)/2);
        glfwSetWindowAspectRatio(m_Window, 16, 9);

        glfwMakeContextCurrent(m_Window);

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        {
            LOG_ERROR("Failed to initialize GLAD!");
            glfwTerminate();
            return false;
        }

        glfwSetWindowUserPointer(m_Window, this);
        glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);
        glfwSetKeyCallback(m_Window, key_callback);
        glfwSetInputMode(m_Window, GLFW_STICKY_KEYS, GLFW_TRUE);
        glfwSetMouseButtonCallback(m_Window, mouse_button_callback);
        glfwSetScrollCallback(m_Window, scroll_callback);
        glfwSetCursorPosCallback(m_Window, cursor_position_callback);

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(opengl_error_callback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
		
        glViewport(0, 0, m_Width, m_Height);

        return true;
    }

    void Window::closeWindow(void) const
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void Window::clearWindow(void) const
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Window::updateWindow(void) const
    {
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }

    bool Window::windowShouldClose(void) const
    {
        if (glfwWindowShouldClose(m_Window)) { return true; }

        return false;
    }

    void Window::error_callback(int code, const char* description)
    {
        LOG_ERROR(code, description);
    }

    void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        Window* win = (Window*)glfwGetWindowUserPointer(window);
        glViewport(0, 0, width, height);
        win->m_Width = width;
        win->m_Height = height;
        LOG_TRACE("Window resizing:", win->m_Width, "x", win->m_Height);
    }

    void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        KeyboardInput::getInstance().key_callback(window, key, scancode, action, mods);
    }
                
    void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
    {
        MouseInput::getInstance().cursor_position_callback(window, xpos, ypos);
    }

    void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        MouseInput::getInstance().scroll_callback(window, xoffset, yoffset);
    }

    void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        MouseInput::getInstance().mouse_button_callback(window, button, action, mods);
    }

    void Window::opengl_error_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
        if (id == 131185 || id == 131218) return; // Annoying Nvidia driver messages

        switch (severity)
        {   
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                LOG_WARN("OpenGL NOTIFICATION:", message);
                break;

            default:
                LOG_ERROR("OpenGL ERROR:", message);
                break;
        }
    }

}