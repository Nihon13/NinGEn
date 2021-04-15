#include "Core/core.h"
#include "Core/window.h"
#include "Math/math.h"
#include "Graphics/shader.h"
#include "Graphics/vbo.h"
#include "Graphics/ibo.h"
#include "Graphics/vao.h"
#include "Graphics/texture.h"

int main()
{
    using namespace ningen;

    Window window;

    glClearColor(0.0f, 0.3f, 0.4f, 1.0f);

    Shader shader("../res/Shaders/vertex_shader.glsl", "../res/Shaders/fragment_shader.glsl");

    Texture tex1("../res/Textures/tekstura.png");

    float vertices[] = 
    {
        -9.0f, -16.0f, 0.0f,
         9.0f, -16.0f, 0.0f,
         9.0f,  16.0f, 0.0f,
        -9.0f,  16.0f, 0.0f
    };

    float uv[] =
    {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    unsigned int indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    VAO vao;
    VBO vbo(vertices, 4 * 3, 3);
    VBO vbo_uv(uv, 4 * 2, 2);
    vbo.bind();
    vbo_uv.bind();
    IBO ibo(indices, 6);
    ibo.bind();
    vao.addVbo(&vbo, 0);
    vao.addVbo(&vbo_uv, 1);
    vbo.unbind();
    vbo_uv.unbind();

    shader.start();
    tex1.bind();
    shader.setUniform1i("u_Texture", 0);

    Mat4 mvp = Mat4::orthographic(-100.0f, 100.0f, -100.0f, 100.0f, -1.0f, 1.0f);
    Mat4 matrix = Mat4::translate(Vec3(30.0f, 20.0f, 0.0f));

    shader.setUniformMat4f("u_ProjectionMatrix", mvp);

    while (!window.windowShouldClose())
    {
        window.clearWindow();
        vao.bind();
        ibo.bind();
        shader.setUniformMat4f("u_ModelMatrix", matrix);
        glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_INT, nullptr);
        ibo.unbind();
        vao.unbind();
        window.updateWindow();
    }

    return 0;
}