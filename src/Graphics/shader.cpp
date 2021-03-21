#include "shader.h"

namespace ningen {

    Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
        : m_ShaderID(0)
    {
        std::string vertexDataBuff = readFile(vertexShaderPath);
        std::string fragmentDataBuff = readFile(fragmentShaderPath);

        const char* vertexData = vertexDataBuff.c_str();
        const char* fragmentData = fragmentDataBuff.c_str();

        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(vertexShader, 1, &vertexData, NULL);
        glShaderSource(fragmentShader, 1, &fragmentData, NULL);

        glCompileShader(vertexShader);
        checkErrors(vertexShader, "Vertex");

        glCompileShader(fragmentShader);
        checkErrors(fragmentShader, "Fragment");

        m_ShaderID = glCreateProgram();

        glAttachShader(m_ShaderID, vertexShader);
        glAttachShader(m_ShaderID, fragmentShader);

        glLinkProgram(m_ShaderID);
        checkErrors(m_ShaderID, "Program");
        
        glDeleteProgram(vertexShader);
        glDeleteProgram(fragmentShader);
    }

    Shader::~Shader(void)
    {
        stop();
        glDeleteProgram(m_ShaderID);
    }

    void Shader::start(void) const
    {
        glUseProgram(m_ShaderID);
    }

    void Shader::stop(void) const
    {
        glUseProgram(0);
    }

    void Shader::checkErrors(unsigned int shader, const char* type) const
    {
        int success = 0;
        if (type != "Program")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (success == GL_FALSE)
            {
                LOG_ERROR(type, "shader compilation error!");
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (success == GL_FALSE)
            {
                LOG_ERROR(type, "linking error!");
            }
        }
    }

}