#ifndef MODEL_H
#define MODEL_H

#include "Graphics/mesh.h"
#include "Graphics/texture.h"
#include "Graphics/skeletalanimation.h"
#include <fstream>

#define NHMF_VERSION "0005"

namespace ningen {

    class Model
    {
        private:
            const char* m_Path;
            std::vector<Mesh*> m_Meshes;
            std::vector<SkeletalAnimation> m_Animations;
            bool loadMeshes(void);
        public:
            Model(const char* path);
            ~Model(void);
            
            const Mesh& getMesh(unsigned int index) const { return *m_Meshes[index]; }
            // const SkeletalAnimation& getAnim(unsigned int index) const { return m_Animations[index]; }
    };

}

#endif