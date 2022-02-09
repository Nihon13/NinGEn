#ifndef TEMPMODEL
#define TEMPMODEL

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Graphics/mesh.h"
#include "Graphics/texture.h"
#include <fstream>

namespace ningen {

    class TempModel
    {
        private:
            const char* m_Path;
            std::vector<Mesh*> m_Meshes;
            const aiScene* m_Scene;
            unsigned int m_NumMeshes = 0;
            std::vector<std::vector<Vertex>> m_Vertices;
            std::vector<std::vector<unsigned int>> m_Indices;
            bool initAssimp(void);
            void extractMeshData(const aiMesh* mesh);
        public:
            TempModel(const char* path);
            ~TempModel(void);
            const Mesh& getMesh(unsigned int index) const { return *m_Meshes[index]; }

            inline unsigned int getNumMeshes(void) const { return m_NumMeshes; }
    };

}

#endif