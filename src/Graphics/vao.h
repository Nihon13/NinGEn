#ifndef VAO_H
#define VAO_H

#include "Graphics/vbo.h"
#include <vector>

namespace ningen {

    class VAO
    {
        private:
            GLuint m_VaoID;
            std::vector<VBO*> m_Vbos;
        public:
            VAO(void);
            ~VAO(void);
            void bind(void) const;
            void unbind(void) const;
            void addVbo(VBO* vbo, GLuint index);
    };

}

#endif