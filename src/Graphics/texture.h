#ifndef TEXTURE_H
#define TEXTURE_H

#include "Core/core.h"
#include "stb/stb_image.h"
#include "glad/glad.h"

namespace ningen {

    class Texture
    {
        private:
            unsigned int m_TextureID;
            unsigned char* m_Data;
            int m_Width;
            int m_Height;
            int m_Channels;
        public:
            Texture(const char* path);
            ~Texture(void);
            void bind(unsigned int slot = 0) const;
            void unbind(void) const;

            inline int getWidth(void) const { return m_Width; };
            inline int getHeight(void) const { return m_Height; };
            inline int getChannels(void) const { return m_Channels; };
    };

}

#endif