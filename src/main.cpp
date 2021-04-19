#include "Core/core.h"
#include "Core/window.h"
#include "Math/math.h"
#include "Graphics/shader.h"
#include "Graphics/vbo.h"
#include "Graphics/ibo.h"
#include "Graphics/vao.h"
#include "Graphics/texture.h"
#include "Graphics/mesh.h"

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

    std::vector<Vertex> verts;
    std::vector<unsigned int> inds;

    Vertex vert;

    for (int i = 0, j = 0; i < sizeof(vertices)/sizeof(vertices[0]); i+=3, j+=2)
    {
        vert.position.x = vertices[i];
        vert.position.y = vertices[i+1];
        vert.position.z = vertices[i+2];

        vert.uv.x = uv[j];
        vert.uv.y = uv[j+1];

        verts.push_back(vert);
    }

    for (int i = 0; i < sizeof(indices)/sizeof(indices[0]); i++)
    {
        inds.push_back(indices[i]);
    }

    Mesh mesh(verts, inds, tex1);

    shader.start();
    tex1.bind();
    shader.setUniform1i("u_Texture", 0);

    Mat4 mvp = Mat4::orthographic(-100.0f, 100.0f, -100.0f, 100.0f, -1.0f, 1.0f);
    Mat4 matrix = Mat4::translate(Vec3(30.0f, 20.0f, 0.0f));

    shader.setUniformMat4f("u_ProjectionMatrix", mvp);

    while (!window.windowShouldClose())
    {
        window.clearWindow();
        shader.setUniformMat4f("u_ModelMatrix", matrix);
        mesh.draw();
        window.updateWindow();
    }

    return 0;
}