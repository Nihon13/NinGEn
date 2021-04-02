#include "ibo.h"

namespace ningen {

    IBO::IBO(GLuint* data, GLuint count, int mode)
        : m_Count(count)
    {
        glGenBuffers(1, &m_IboID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IboID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, mode);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    IBO::~IBO(void)
    {
        glDeleteBuffers(1, &m_IboID);
    }

    void IBO::bind(void) const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IboID);
    }

    void IBO::unbind(void) const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

}