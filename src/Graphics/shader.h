#ifndef SHADER_H
#define SHADER_H

#include "Core/core.h"
#include "Utils/fileutils.h"
#include "glad/glad.h"

namespace ningen {

    class Shader
    {
        private:
            unsigned int m_ShaderID;
            void checkErrors(unsigned int shader, const char* type) const;
        public:
            Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
            ~Shader(void);
            void start(void) const;
            void stop(void) const;
    };

}

#endif