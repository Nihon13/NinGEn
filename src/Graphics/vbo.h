#ifndef VBO_H
#define VBO_H

#include "glad/glad.h"

namespace ningen {

    class VBO
    {
        private:
            GLuint m_VboID;
            GLuint m_ComponentCount;
        public:
            VBO(GLfloat* data, GLuint count, GLuint componentCount, int mode = GL_STATIC_DRAW);
            ~VBO(void);
            void bind(void) const;
            void unbind(void) const;
            inline GLuint getComponentCount(void) const { return m_ComponentCount; }
    };

}

#endif