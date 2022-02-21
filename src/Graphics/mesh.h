#ifndef MESH_H
#define MESH_H

#include "Math/math.h"
#include "Graphics/texture.h"
#include <vector>

#define MAX_BONE_INFLUENCE 3

namespace ningen {

    struct Vertex
    {
        Vec3 position;
        Vec3 normal;
        Vec2 uv;
        Vec3 bitangent;
        Vec3 tangent;
        int boneIDs[MAX_BONE_INFLUENCE];
        float weights[MAX_BONE_INFLUENCE];
    };

    class Mesh
    {
        private:
            std::vector<Vertex> m_Vertices;
            std::vector<unsigned int> m_Indices;
            GLuint m_Vao;
            GLuint m_Vbo;
            GLuint m_Ibo;
            void init(int mode = GL_STATIC_DRAW);
        public:
            Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
            ~Mesh(void);
            void draw(void) const;
    };

}

#endif