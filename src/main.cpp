#include "Core/core.h"
#include "Core/window.h"
#include "Math/math.h"
#include "Graphics/shader.h"
#include "Graphics/vbo.h"
#include "Graphics/ibo.h"
#include "Graphics/vao.h"
#include "Graphics/texture.h"
#include "Graphics/model.h"

int main()
{
    using namespace ningen;

    Window window;

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    Shader shader("../res/Shaders/vertex_shader.glsl", "../res/Shaders/fragment_shader.glsl");

    Texture tex1("../../NinGEnModelLoader/res/Sciana.png");

    Model robot1("../../NinGEnModelLoader/res/Robot1.nhmf");
    Model interior("../../NinGEnModelLoader/res/Interior.nhmf");

    shader.start();
    tex1.bind(0);

    // Mat4 projection_matrix = Mat4::orthographic(-100.0f, 100.0f, -100.0f, 100.0f, -1.0f, 1.0f);
    Mat4 projection_matrix = Mat4::perspective(45.0f, 16.0f/9.0f, 0.1f, 300.0f);
    Mat4 view_matrix = Mat4::identity();    
    Mat4 model_matrix = Mat4::identity();

    view_matrix = Mat4::translate(Vec3(0.0f, -4.0f, -120.0f)) * Mat4::rotate(30.0f, Vec3(1.0f, 0.0f, 0.0f)) * Mat4::rotate(190.0f, Vec3(0.0f, 1.0f, 0.0f)) * Mat4::scale(Vec3(0.045f, 0.045f, 0.045f));

    shader.setUniformMat4f("u_ProjectionMatrix", projection_matrix);
    shader.setUniformMat4f("u_ViewMatrix", view_matrix);

    while (!window.windowShouldClose())
    {
        window.clearWindow();

        model_matrix = Mat4::identity();
        shader.setUniformMat4f("u_ModelMatrix", model_matrix);
        interior.getMesh(0).draw();
        interior.getMesh(1).draw();
        interior.getMesh(2).draw();
        interior.getMesh(3).draw();
        interior.getMesh(4).draw();
        interior.getMesh(5).draw();
        interior.getMesh(6).draw();

        model_matrix = Mat4::translate(Vec3(0.0f, 0.0f, -500.0f));
        shader.setUniformMat4f("u_ModelMatrix", model_matrix);
        robot1.getMesh(0).draw();
        robot1.getMesh(1).draw();
        robot1.getMesh(2).draw();
        robot1.getMesh(3).draw();
        robot1.getMesh(4).draw();
        robot1.getMesh(5).draw();

        window.updateWindow();
    }

    return 0;
}