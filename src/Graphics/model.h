#ifndef MODEL_H
#define MODEL_H

#include "Graphics/mesh.h"
#include "Graphics/texture.h"
#include <fstream>

#define NHMF_VERSION "0003"

namespace ningen {

    struct Bone
    {
        int id;
        Mat4 offsetMatrix;
    };

    class Model
    {
        private:
            const char* m_Path;
            std::vector<Mesh*> m_Meshes;
            std::vector<Bone> m_Bones;
            bool loadMeshes(void);
        public:
            Model(const char* path);
            ~Model(void);
            
            const Mesh& getMesh(unsigned int index) const { return *m_Meshes[index]; }
    };

}

#endif