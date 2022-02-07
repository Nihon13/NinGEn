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
    Shader simpleShader("../res/Shaders/simple_vertex_shader.glsl", "../res/Shaders/simple_fragment_shader.glsl");

    Texture tex1("../../NinGEnModelLoader/res/Sciana.png");

    // writeToFile("../../NinGEnModelLoader/res/TestV6/Robot.fbx");
    // writeToFile("../../NinGEnModelLoader/res/TestV6/Robot.dae");
    // writeToFile("../../NinGEnModelLoader/res/TestV6/RigTestBox.fbx");
    // writeToFile("../../NinGEnModelLoader/res/TestV6/JointDebug2.fbx");

    Model robot1("../../NinGEnModelLoader/res/TestV6/Robot.nhmf");
    // Model cube("../../NinGEnModelLoader/res/TestV6/Cube.nhmf");
    Model testBox("../../NinGEnModelLoader/res/TestV6/RigTestBox.nhmf");
    Model jointDebug("../../NinGEnModelLoader/res/TestV6/JointDebug2.nhmf");

    // Model interior("../../NinGEnModelLoader/res/Interior.nhmf");

    shader.start();
    tex1.bind(0);

    // Mat4 projection_matrix = Mat4::orthographic(-100.0f, 100.0f, -100.0f, 100.0f, -1.0f, 1.0f);
    Mat4 projection_matrix = glm::perspective(glm::radians(45.0f), 16.0f/9.0f, 0.1f, 300.0f);
    Mat4 view_matrix = Mat4(1.0f);    
    Mat4 model_matrix = Mat4(1.0f);

    view_matrix = glm::translate(view_matrix, Vec3(0.0f, -1.0f, -4.0f));
    // view_matrix = glm::translate(view_matrix, Vec3(0.0f, -0.0f, -4.0f));
    view_matrix = glm::rotate(view_matrix, glm::radians(10.0f), Vec3(1.0f, 0.0f, 0.0f));
    view_matrix = glm::rotate(view_matrix, glm::radians(10.0f), Vec3(0.0f, 1.0f, 0.0f));
    // view_matrix = glm::scale(view_matrix, Vec3(0.09f, 0.09f, 0.09f));

    shader.setUniformMat4f("u_ProjectionMatrix", projection_matrix);
    shader.setUniformMat4f("u_ViewMatrix", view_matrix);

    simpleShader.start();
    Mat4 model_matrix2 = Mat4(1.0f);
    // view_matrix2 = glm::translate(view_matrix2, Vec3(0.0f, 0.0f, -2.0f));
    simpleShader.setUniformMat4f("u_ProjectionMatrix", projection_matrix);
    simpleShader.setUniformMat4f("u_ViewMatrix", view_matrix);
    simpleShader.stop();

    LOG_TRACE(glm::to_string(testBox.getAnimManager().m_Bones[0].offsetMatrix));
    // LOG_TRACE(glm::to_string(testBox.getAnimManager().m_Bones[1].offsetMatrix));

    while (!window.windowShouldClose())
    {
        window.clearWindow();

        shader.start();
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

        // model_matrix = glm::translate(model_matrix, Vec3(0.0f, 0.0f, -500.0f));
        // model_matrix = glm::translate(model_matrix, Vec3(0.0f, 0.0f, 50.0f));
        // model_matrix = glm::scale(model_matrix, Vec3(10.0f, 10.0f, 10.0f));
        // model_matrix = glm::scale(model_matrix, Vec3(0.25f, 0.25f, 0.25f));
        shader.setUniformMat4f("u_ModelMatrix", model_matrix);
        // robot1.getMesh(0).draw();
        // robot1.getMesh(1).draw();
        // robot1.getMesh(2).draw();
        // robot1.getMesh(3).draw();
        // robot1.getMesh(4).draw();
        // robot1.getMesh(5).draw();

        // cube.getMesh(0).draw();

        testBox.getMesh(0).draw();
        testBox.getMesh(1).draw();

        simpleShader.start();
        model_matrix2 = Mat4(1.0f);
        // model_matrix2[0] = Vec4(0.0f, 1.0f, 0.0f, 0.0f);
        // model_matrix2[1] = Vec4(0.0f, 0.0f, 1.0f, 0.0f);
        // model_matrix2[2] = Vec4(1.0f, 0.0f, 0.0f, 0.0f);
        // model_matrix2[3] = Vec4(-1.075882f, 0.4254547f, 0.0f, 1.0f);
        // LOG_TRACE(glm::to_string(model_matrix2));
        // model_matrix2 = testBox.getAnimManager().m_Bones[0].offsetMatrix;// * model_matrix;
        // // model_matrix2 = glm::scale(model_matrix2, Vec3(0.9f, 0.9f, 0.9f));
        // simpleShader.setUniformMat4f("u_ModelMatrix", model_matrix2);
        // jointDebug.getMesh(0).draw();
        // test();

        // model_matrix2 = testBox.getAnimManager().m_Bones[1].offsetMatrix;// * model_matrix;
        // simpleShader.setUniformMat4f("u_ModelMatrix", model_matrix2);
        // jointDebug.getMesh(0).draw();

        // for (int i = 0; i < 6; i++)
        // {
        //     model_matrix2 = robot1.getAnimManager().m_Bones[i].offsetMatrix;
        //     simpleShader.setUniformMat4f("u_ModelMatrix", model_matrix2);
        //     jointDebug.getMesh(0).draw();
        // }

        // for (int i = 0; i < 2; i++)
        // {
            model_matrix2 = testBox.getAnimManager().m_Bones[0].offsetMatrix;
            simpleShader.setUniformMat4f("u_ModelMatrix", model_matrix2);
            jointDebug.getMesh(0).draw();
        // }

            model_matrix2 = testBox.getAnimManager().m_Bones[1].offsetMatrix;
            // // model_matrix2 = glm::rotate(model_matrix2, glm::radians(90.0f), Vec3(1.0f, 0.0f, 0.0f));
            // model_matrix2 = glm::translate(model_matrix2, Vec3(0.4254547f, 1.075882f, 0.0f));
            // model_matrix2 = glm::rotate(model_matrix2, glm::radians(-90.0f), Vec3(1.0f, 0.0f, 0.0f));
            // model_matrix2 = glm::rotate(model_matrix2, glm::radians(90.0f), Vec3(0.0f, 0.0f, 1.0f));


            simpleShader.setUniformMat4f("u_ModelMatrix", model_matrix2);
            jointDebug.getMesh(0).draw();


        simpleShader.stop();

        window.updateWindow();
    }

    return 0;
}

// model_matrix2 = mat4x4((0, 0, 1, 0), (1, 0, 0, 0), (0, 1, 0, 0), (0.425455, 1.075882, 0, 1)) prawidlowa
// model_matrix2 = mat4x4((0, 1, 0, 0), (0, 0, 1, 0), (1, 0, 0, 0), (0, 0.425455, -1.075882, 1)) zla - testBox.getAnimManager().m_Bones[1].offsetMatrix