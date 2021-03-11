#include <iostream>
#include "GLFW/glfw3.h"

int main()
{
    GLFWwindow* window;
    if (!glfwInit())
        return -1;

    std::cout << glfwGetVersionString() << std::endl;

    glfwTerminate();

    return 0;
}