#ifndef IBO_H
#define IBO_H

#include "glad/glad.h"

namespace ningen {

    class IBO
    {
        private:
            GLuint m_IboID;
            GLuint m_Count;
        public:
            IBO(GLuint* data, GLuint count, int mode = GL_STATIC_DRAW);
            ~IBO(void);
            void bind(void) const;
            void unbind(void) const;
            
            inline GLuint getCount(void) const { return m_Count; }
    };

}

#endif