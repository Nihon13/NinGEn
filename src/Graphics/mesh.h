#ifndef MESH_H
#define MESH_H

#include "Math/math.h"
#include "Graphics/texture.h"
#include "Graphics/vao.h"
#include "Graphics/vbo.h"
#include "Graphics/ibo.h"
#include <vector>

namespace ningen {

    struct Vertex
    {
        Vec3 position;
        Vec3 normal;
        Vec2 uv;
        Vec3 binormal;
        Vec3 tangent;
    };

    class Mesh
    {
        private:
            std::vector<Vertex> m_Vertices;
            std::vector<unsigned int> m_Indices;
            Texture m_Texture;
            GLuint m_Vao;
            GLuint m_Vbo;
            GLuint m_Ibo;
            void init(int mode = GL_STATIC_DRAW);
        public:
            Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const Texture& texture);
            ~Mesh(void);
            void draw(void) const;
    };

}

#endif