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

    // Model robot1("../../NinGEnModelLoader/res/TestV6/Robot.nhmf");
    // Model testBox("../../NinGEnModelLoader/res/TestV6/RigTestBox.nhmf");
    // Model jointDebug("../../NinGEnModelLoader/res/TestV6/JointDebug2.nhmf");

    Model robotTestModel("../../NinGEnModelLoader/res/Kontynuacja/Robot.nhmf");
    Model robotTestModel1("../../NinGEnModelLoader/res/Kontynuacja/Robot1.nhmf");

    shader.start();
    tex1.bind(0);

    // Mat4 projection_matrix = Mat4::orthographic(-100.0f, 100.0f, -100.0f, 100.0f, -1.0f, 1.0f);
    Mat4 projection_matrix = glm::perspective(glm::radians(45.0f), 16.0f/9.0f, 0.1f, 300.0f);
    Mat4 view_matrix = Mat4(1.0f);    
    Mat4 model_matrix = Mat4(1.0f);

    view_matrix = glm::translate(view_matrix, Vec3(0.0f, -10.0f, -50.0f));
    // view_matrix = glm::translate(view_matrix, Vec3(0.0f, -20.0f, -130.0f));
    // view_matrix = glm::rotate(view_matrix, glm::radians(-90.0f), Vec3(1.0f, 0.0f, 0.0f));
    view_matrix = glm::rotate(view_matrix, glm::radians(90.0f), Vec3(0.0f, 1.0f, 0.0f));

    shader.setUniformMat4f("u_ProjectionMatrix", projection_matrix);
    shader.setUniformMat4f("u_ViewMatrix", view_matrix);

    float timer = glfwGetTime();

    while (!window.windowShouldClose())
    {
        window.clearWindow();

        shader.start();
        model_matrix = Mat4(1.0f);

        timer = glfwGetTime();

        robotTestModel.calculateBoneTransform(timer);

        for (int i = 0; i < robotTestModel.getBonesNum(); i++)
        {
            std::string s = "u_FinalBonesMatrices[" + std::to_string(i) + "]";
            shader.setUniformMat4f(s.c_str(), robotTestModel.getFinalBonesMatrices(i));
        }

        shader.setUniformMat4f("u_ModelMatrix", model_matrix);

        for (unsigned int i = 0; i < robotTestModel.getNumMeshes(); i++)
        {
            robotTestModel.getMesh(i).draw();
        }

        ////////////////////////////
        robotTestModel1.calculateBoneTransform(timer);

        for (int i = 0; i < robotTestModel.getBonesNum(); i++)
        {
            std::string s = "u_FinalBonesMatrices[" + std::to_string(i) + "]";
            shader.setUniformMat4f(s.c_str(), robotTestModel1.getFinalBonesMatrices(i));
        }

        model_matrix = glm::translate(model_matrix, Vec3(0.0f, 0.0f, 20.0f));

        shader.setUniformMat4f("u_ModelMatrix", model_matrix);

        for (unsigned int i = 0; i < robotTestModel1.getNumMeshes(); i++)
        {
            robotTestModel1.getMesh(i).draw();
        }

        window.updateWindow();
    }

    return 0;
}