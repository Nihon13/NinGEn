#include "texture.h"

namespace ningen {

    Texture::Texture(const char* path)
        : m_TextureID(0), m_Data(nullptr), m_Width(0), m_Height(0), m_Channels(0)
    {
        stbi_set_flip_vertically_on_load(true);
        m_Data = stbi_load(path, &m_Width, &m_Height, &m_Channels, 4);

        glGenTextures(1, &m_TextureID);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Data);

        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);

        if (m_Data == NULL)
        {
            LOG_ERROR("Failed to load texture: [", path, "]");
        }
        else
        {
            LOG_INFO("Texture loaded: [", path, "]");
        }
        
        stbi_image_free(m_Data); 
    }

    Texture::~Texture(void)
    {
        glDeleteTextures(1, &m_TextureID);
    }

    void Texture::bind(unsigned int slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
    }

    void Texture::unbind(void) const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}