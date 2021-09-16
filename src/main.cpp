#include "Core/core.h"
#include "Core/window.h"
#include "Math/math.h"
#include "Graphics/shader.h"
#include "Graphics/vbo.h"
#include "Graphics/ibo.h"
#include "Graphics/vao.h"
#include "Graphics/texture.h"
#include "Graphics/model.h"

#include "Graphics/writer.h"

int main()
{
    using namespace ningen;

    Window window;

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    Shader shader("../res/Shaders/vertex_shader.glsl", "../res/Shaders/fragment_shader.glsl");

    Texture tex1("../../NinGEnModelLoader/res/Sciana.png");

    // writeToFile("../../NinGEnModelLoader/res/TestV6/Robot.fbx");
    Model robot1("../../NinGEnModelLoader/res/TestV6/Robot.nhmf");

    // Model interior("../../NinGEnModelLoader/res/Interior.nhmf");

    shader.start();
    tex1.bind(0);

    // Mat4 projection_matrix = Mat4::orthographic(-100.0f, 100.0f, -100.0f, 100.0f, -1.0f, 1.0f);
    Mat4 projection_matrix = glm::perspective(glm::radians(45.0f), 16.0f/9.0f, 0.1f, 300.0f);
    Mat4 view_matrix = Mat4(1.0f);    
    Mat4 model_matrix = Mat4(1.0f);

    view_matrix = glm::translate(view_matrix, Vec3(0.0f, 0.0f, -85.0f));
    view_matrix = glm::rotate(view_matrix, glm::radians(30.0f), Vec3(1.0f, 0.0f, 0.0f));
    view_matrix = glm::rotate(view_matrix, glm::radians(190.0f), Vec3(0.0f, 1.0f, 0.0f));
    view_matrix = glm::scale(view_matrix, Vec3(0.045f, 0.045f, 0.045f));

    shader.setUniformMat4f("u_ProjectionMatrix", projection_matrix);
    shader.setUniformMat4f("u_ViewMatrix", view_matrix);

    while (!window.windowShouldClose())
    {
        window.clearWindow();

        model_matrix = Mat4(1.0f);
        // shader.setUniformMat4f("u_ModelMatrix", model_matrix);
        // interior.getMesh(0).draw();
        // interior.getMesh(1).draw();
        // interior.getMesh(2).draw();
        // interior.getMesh(3).draw();
        // interior.getMesh(4).draw();
        // interior.getMesh(5).draw();
        // interior.getMesh(6).draw();

        auto transforms = robot1.getAnimManager().getFinalBonesMatrices();
        for (int i = 0; i < robot1.getAnimManager().getBonesCount(); i++)
        {
            std::string s = "u_FinalBonesMatrices[" + std::to_string(i) + "]";
            shader.setUniformMat4f(s.c_str(), transforms[i]);
        }

        model_matrix = glm::translate(model_matrix, Vec3(0.0f, 0.0f, -500.0f));
        model_matrix = glm::scale(model_matrix, Vec3(0.25f, 0.25f, 0.25f));
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