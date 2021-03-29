#include "vbo.h"

namespace ningen {

    VBO::VBO(GLfloat* data, GLuint count, GLuint componentCount, int mode)
        : m_ComponentCount(componentCount)
    {
        glGenBuffers(1, &m_VboId);
        glBindBuffer(GL_ARRAY_BUFFER, m_VboId);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(GLfloat), data, mode);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    VBO::~VBO(void)
    {
        glDeleteBuffers(1, &m_VboId);
    }

    void VBO::bind(void) const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_VboId);
    }

    void VBO::unbind(void) const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

}