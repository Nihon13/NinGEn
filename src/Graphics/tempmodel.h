#ifndef TEMPMODEL
#define TEMPMODEL

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Graphics/mesh.h"
#include "Graphics/texture.h"
#include <fstream>
#include <algorithm>

namespace ningen {

    struct Bone
    {
        int id;
        std::string name;
        Mat4 offsetMatrix;
        Mat4 transformMatrix;
        int parentBoneID;
    };

    struct KeyPos
    {
        float position[3];
        double time;
    };

    struct KeyRot
    {
        float rotation[4];
        double time;
    };

    struct KeyScale
    {
        float scale[3];
        double time;
    };

    struct AnimNode
    {
        int id;
        std::vector<KeyPos> positionsKeys;
        std::vector<KeyRot> rotationsKeys;
        std::vector<KeyScale> scalingsKeys;
    };

    struct Animation
    {
        std::string name;
        double duration;
        double ticksPerSecond;
        std::vector<AnimNode> channels;
    };

    class TempModel
    {
        private:
            const char* m_Path;
            std::vector<Mesh*> m_Meshes;
            const aiScene* m_Scene;
            unsigned int m_NumMeshes = 0;
            std::vector<std::vector<Vertex>> m_Vertices;
            std::vector<std::vector<unsigned int>> m_Indices;
            std::vector<std::string> m_BonesNamesList;
            std::vector<Bone> m_Bones;
            std::vector<Animation> m_Animations;
            Mat4 m_RootFinalTransformation = Mat4(1.0f);
            std::vector<Mat4> m_FinalBonesMatrices;
            bool initAssimp(void);
            void extractMeshData(const aiMesh* mesh);
            void extractBonesNames(const aiMesh* mesh);
            void extractBoneWeights(const aiMesh* mesh, std::vector<Vertex>& vertices);
            void addBoneToList(const aiBone* bone);
            void readNodesHierarchy(const aiNode* node, int parentID, Mat4& parentTrasformation);
            int findBoneId(std::string name);
            void extractAnimInfo(const aiAnimation* anim);
            int findAnimBone(int animID, int boneID) const;
        public:
            TempModel(const char* path);
            ~TempModel(void);
            void getBoneTransform(float timeInSec);
            const Mesh& getMesh(unsigned int index) const { return *m_Meshes[index]; }
            const Mat4& getFinalBonesMatrices(unsigned int index) const { return m_FinalBonesMatrices[index]; }

            inline const unsigned int getBonesNum(void) const { return m_Bones.size(); }
            inline unsigned int getNumMeshes(void) const { return m_NumMeshes; }
            inline static Mat4 AssimpToGLM(const aiMatrix4x4 &matrix)
            {
                return Mat4(
                    matrix[0][0], matrix[1][0], matrix[2][0], matrix[3][0],
                    matrix[0][1], matrix[1][1], matrix[2][1], matrix[3][1],
                    matrix[0][2], matrix[1][2], matrix[2][2], matrix[3][2],
                    matrix[0][3], matrix[1][3], matrix[2][3], matrix[3][3]
                );
            }
    };

}

#endif