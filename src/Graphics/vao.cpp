#include "vao.h"

namespace ningen {

    VAO::VAO(void)
    {
        glGenVertexArrays(1, &m_VaoID);
    }

    VAO::~VAO(void)
    {
        m_Vbos.clear();

        glDeleteVertexArrays(1, &m_VaoID);
    }

    void VAO::bind(void) const
    {
        glBindVertexArray(m_VaoID);
    }

    void VAO::unbind(void) const
    {
        glBindVertexArray(0);
    }

    void VAO::addVbo(VBO* vbo, GLuint index)
    {
        bind();
        vbo->bind();

        glVertexAttribPointer(index, vbo->getComponentCount(), GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(index);

        vbo->unbind();
        unbind();

        m_Vbos.push_back(vbo);
    }

}