#ifndef SHADER_H
#define SHADER_H

#include "Core/core.h"
#include "Utils/fileutils.h"
#include "Math/math.h"
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
            void setUniform1i(const char* name, int value) const;
            void setUniform1f(const char* name, float value) const;
            void setUniform3f(const char* name, const Vec3& vector3) const;
            void setUniformMat4f(const char* name, const Mat4& matrix) const;
    };

}

#endif