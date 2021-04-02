#include "vbo.h"

namespace ningen {

    VBO::VBO(GLfloat* data, GLuint count, GLuint componentCount, int mode)
        : m_ComponentCount(componentCount)
    {
        glGenBuffers(1, &m_VboID);
        glBindBuffer(GL_ARRAY_BUFFER, m_VboID);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(GLfloat), data, mode);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    VBO::~VBO(void)
    {
        glDeleteBuffers(1, &m_VboID);
    }

    void VBO::bind(void) const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_VboID);
    }

    void VBO::unbind(void) const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

}