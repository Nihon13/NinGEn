#include "Core/core.h"
#include "Core/window.h"
#include "Graphics/shader.h"
#include "Graphics/vbo.h"
#include "Graphics/ibo.h"
#include "Graphics/vao.h"
#include "Math/math.h"

int main()
{
    using namespace ningen;

    Window window;

    glClearColor(0.0f, 0.3f, 0.4f, 1.0f);

    Shader shader("../res/Shaders/vertex_shader.glsl", "../res/Shaders/fragment_shader.glsl");

    float vertices[] = 
    {
        -30.5f, -30.5f, 0.0f,
         30.5f, -30.5f, 0.0f,
         30.5f,  30.5f, 0.0f,
        -30.5f,  30.5f, 0.0f
    };

    unsigned int indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    VAO vao;
    VBO vbo(vertices, 4 * 3, 3);
    vbo.bind();
    IBO ibo(indices, 6);
    ibo.bind();
    vao.addVbo(&vbo, 0);

    shader.start();

    Mat4 mvp = Mat4::orthographic(-100.0f, 100.0f, -100.0f, 100.0f, -1.0f, 1.0f);
    Mat4 matrix = Mat4::translate(Vec3(30.0f, 20.0f, 0.0f));

    shader.setUniformMat4f("projection_matrix", mvp);

    while (!window.windowShouldClose())
    {
        window.clearWindow();
        vao.bind();
        ibo.bind();
        shader.setUniformMat4f("model_matrix", matrix);
        glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_INT, nullptr);
        ibo.unbind();
        vao.unbind();
        window.updateWindow();
    }

    return 0;
}