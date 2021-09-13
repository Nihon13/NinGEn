#ifndef SKELETALANIMATIONMANAGER_H
#define SKELETALANIMATIONMANAGER_H

#include "Math/math.h"
#include <string>
#include <vector>

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

    class SkeletalAnimationManager
    {
        public:
            std::vector<Mat4>& getFinalBonesMatrices(void) { return m_FinalBonesMatrices; }
            int getBonesCount(void) const { return m_BonesCount; }
        private:
            std::vector<Bone> m_Bones;
            std::vector<Animation> m_Animations;
            std::vector<Mat4> m_FinalBonesMatrices;
            int m_BonesCount;
            friend class Model;
    };

}

#endif