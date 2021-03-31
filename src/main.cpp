#include "Core/core.h"
#include "Core/window.h"
#include "Graphics/shader.h"
#include "Graphics/vbo.h"
#include "Graphics/ibo.h"

int main()
{
    using namespace ningen;

    Window window;

    glClearColor(0.0f, 0.3f, 0.4f, 1.0f);

    Shader shader("../res/Shaders/vertex_shader.glsl", "../res/Shaders/fragment_shader.glsl");

    float vertices[] = 
    {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    unsigned int indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    VBO vbo(vertices, 4 * 3, 3);
    vbo.bind();
    IBO ibo(indices, 6);
    ibo.bind();

    glVertexAttribPointer(0, vbo.getComponentCount(), GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
    glEnableVertexAttribArray(0);

    shader.start();

    while (!window.windowShouldClose())
    {
        window.clearWindow();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        window.updateWindow();
    }

    return 0;
}