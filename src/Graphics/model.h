#ifndef MODEL_H
#define MODEL_H

#include "Graphics/mesh.h"
#include <fstream>

#define NHMF_VERSION "0006"

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
        Vec3 position;
        double time;
    };

    struct KeyRot
    {
        Quaternion rotation;
        double time;
    };

    struct KeyScale
    {
        Vec3 scale;
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

    class Model
    {
        private:
            const char* m_Path;
            unsigned int m_NumMeshes = 0;
            std::vector<Mesh*> m_Meshes;
            std::vector<Bone> m_Bones;
            std::vector<Mat4> m_FinalBonesMatrices;
            std::vector<Animation> m_Animations;
            Mat4 m_RootFinalTransformation = Mat4(1.0f);

            bool loadMeshes(void);
            int findAnimBone(int animID, int boneID) const;
            Mat4 interpolatePosition(float animationTime, AnimNode* animNode);
            Mat4 interpolateRotation(float animationTime, AnimNode* animNode);
            Mat4 interpolateScaling(float animationTime, AnimNode* animNode);
            int getPositionIndex(float animationTime, AnimNode* animNode) const;
            int getRotationIndex(float animationTime, AnimNode* animNode) const;
            int getScaleIndex(float animationTime, AnimNode* animNode) const;
            float getScaleFactor(float previousTime, float nextTime, float animationTime) const;
        public:
            Model(const char* path);
            ~Model(void);
            
            void calculateBoneTransform(float timeInSec);

            inline const Mesh& getMesh(unsigned int index) const { return *m_Meshes[index]; }
            inline const Mat4& getFinalBonesMatrices(unsigned int index) const { return m_FinalBonesMatrices[index]; }
            inline const unsigned int getBonesNum(void) const { return m_Bones.size(); }
            inline const unsigned int getNumMeshes(void) const { return m_NumMeshes; }
    };

}

#endif