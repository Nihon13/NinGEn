#include "model.h"

namespace ningen {

    Model::Model(const char* path)
        : m_Path(path)
    {
        if (!loadMeshes())
        {
            exit(EXIT_FAILURE);
        }
        else
        {
            LOG_INFO("Loaded model: [", m_Path, "]");
        }
    }

    Model::~Model(void)
    {
        for (int i = 0; i < m_Meshes.size(); i++)
        {
            delete m_Meshes[i];
        }
    }

    bool Model::loadMeshes()
    {
        std::ifstream file(m_Path, std::ios::out | std::ios::binary);

        if (!file)
        {
            LOG_ERROR("Failed to load a model! [", m_Path, "]");
            return false;
        }

        size_t headerSize;
        size_t versionSize;

        file.read((char*)&headerSize, sizeof(headerSize));
        char* header = new char[headerSize];
        file.read(header, headerSize);

        delete [] header;

        file.read((char*)&versionSize, sizeof(versionSize));
        char* version = new char[versionSize];
        file.read(version, versionSize);

        if (strcmp(NHMF_VERSION, version) != 0)
        {
            LOG_ERROR("Wrong file format version! [", m_Path, "]");
            exit(EXIT_FAILURE);    
        }

        delete [] version;

        size_t meshesCount;
        file.read((char*)&meshesCount, sizeof(meshesCount));
        m_NumMeshes = meshesCount;

        for (unsigned int i = 0; i < meshesCount; i++)
        {
            size_t verticesCount;
            size_t indicesCount;

            file.read((char*)&verticesCount, sizeof(verticesCount));
            file.read((char*)&indicesCount, sizeof(indicesCount));

            std::vector<Vertex> verts(verticesCount);
            std::vector<unsigned int> inds(indicesCount);

            file.read((char*)&verts[0], verticesCount * sizeof(Vertex));
            file.read((char*)&inds[0], indicesCount * sizeof(unsigned int));

            m_Meshes.push_back(new Mesh(verts, inds));
        }

        size_t bonesCount;
        file.read((char*)&bonesCount, sizeof(bonesCount));

        m_FinalBonesMatrices.resize(bonesCount);

        if (bonesCount > 0)
        {
            for (int i = 0; i < bonesCount; i++)
            {
                m_FinalBonesMatrices[i] = Mat4(1.0f);

                Bone bone;
                size_t size;

                file.read((char*)&bone.id, sizeof(((Bone*)0)->id));
                file.read((char*)&size, sizeof(size));
                char* buff = new char[size];
                file.read(buff, size);
                bone.name = buff;
                delete [] buff;
                file.read((char*)&bone.offsetMatrix, sizeof(((Bone*)0)->offsetMatrix));
                file.read((char*)&bone.transformMatrix, sizeof(((Bone*)0)->transformMatrix));
                file.read((char*)&bone.parentBoneID, sizeof(((Bone*)0)->parentBoneID));

                m_Bones.push_back(bone);
            }

            file.read((char*)&m_RootFinalTransformation, sizeof(m_RootFinalTransformation));

            size_t animationsCount;
            file.read((char*)&animationsCount, sizeof(animationsCount));
    
            for (int i = 0; i < animationsCount; i++)
            {
                Animation animation;
                size_t size;

                file.read((char*)&size, sizeof(size));
                char* buff = new char[size];
                file.read(buff, size);
                animation.name = buff;
                delete [] buff;
                file.read((char*)&animation.duration, sizeof(((Animation*)0)->duration));
                file.read((char*)&animation.ticksPerSecond, sizeof(((Animation*)0)->ticksPerSecond));

                size_t numChannels;
                file.read((char*)&numChannels, sizeof(numChannels));

                for (int j = 0; j < numChannels; j++)
                {
                    AnimNode node;

                    file.read((char*)&node.id, sizeof(((AnimNode*)0)->id));

                    size_t numPosKeys;
                    size_t numRotKeys;
                    size_t numScaleKeys;
                    
                    file.read((char*)&numPosKeys, sizeof(numPosKeys));
                    file.read((char*)&numRotKeys, sizeof(numRotKeys));
                    file.read((char*)&numScaleKeys, sizeof(numScaleKeys));
                                        
                    for (int k = 0; k < numPosKeys; k++)
                    {
                        KeyPos kp;
                        file.read((char*)&kp, sizeof(KeyPos));
                        node.positionsKeys.push_back(kp);
                    }

                    for (int k = 0; k < numRotKeys; k++)
                    {
                        KeyRot kr;
                        file.read((char*)&kr, sizeof(KeyRot));
                        node.rotationsKeys.push_back(kr);
                    }

                    for (int k = 0; k < numScaleKeys; k++)
                    {
                        KeyScale ks;
                        file.read((char*)&ks, sizeof(KeyScale));
                        node.scalingsKeys.push_back(ks);
                    }

                    animation.channels.push_back(node);
                }

                m_Animations.push_back(animation);
            }
        }

        file.close();

        return true;
    }

    void Model::calculateBoneTransform(float timeInSec)
    {
        int animationIndex = 0; // TODO: tymczasowo
        float timeInTicks = timeInSec * (float)m_Animations[animationIndex].ticksPerSecond;
        float animationTimeTicks = fmod(timeInTicks, (float)m_Animations[animationIndex].duration);

        for (int i = 0; i < m_Bones.size(); i++)
        {
            Mat4 boneTransform = m_Bones[i].transformMatrix;

            int nodeIndex = findAnimBone(animationIndex, m_Bones[i].id);
            
            if (nodeIndex != -1)
            {
                AnimNode* animNode = &(m_Animations[animationIndex].channels[nodeIndex]);
                Mat4 translate = interpolatePosition(animationTimeTicks, animNode);
                Mat4 rotate = interpolateRotation(animationTimeTicks, animNode);
                Mat4 scale = interpolateScaling(animationTimeTicks, animNode);
                
                boneTransform = translate * rotate * scale;
            }

            if (m_Bones[i].parentBoneID == -1)
            {
                m_FinalBonesMatrices[i] = m_RootFinalTransformation * boneTransform;
            }
            else
            {
                m_FinalBonesMatrices[i] = m_FinalBonesMatrices[m_Bones[i].parentBoneID] * boneTransform;
            }
        }

        for (int i = 0; i < m_Bones.size(); i++)
        {
            m_FinalBonesMatrices[i] = m_FinalBonesMatrices[i] * m_Bones[i].offsetMatrix;
        }
    }

    int Model::findAnimBone(int animID, int boneID) const
    {
        for (int i = 0; i < m_Animations[animID].channels.size(); i++)
        {
            if (m_Animations[animID].channels[i].id == boneID)
            {
                return i;
            }
        }

        return -1;
    }

    Mat4 Model::interpolatePosition(float animationTime, AnimNode* animNode)
    {
        if (animNode->positionsKeys.size() == 1)
        {
            return glm::translate(Mat4(1.0f), animNode->positionsKeys[0].position);
        }

        int p0 = getPositionIndex(animationTime, animNode);
        int p1 = p0 + 1;
        float scaleFactor = getScaleFactor(animNode->positionsKeys[p0].time, animNode->positionsKeys[p1].time, animationTime);
    
        Vec3 finalPos = glm::mix(animNode->positionsKeys[p0].position, animNode->positionsKeys[p1].position, scaleFactor);

        return glm::translate(Mat4(1.0f), finalPos);
    }

    Mat4 Model::interpolateRotation(float animationTime, AnimNode* animNode)
    {
        if (animNode->rotationsKeys.size() == 1)
        {
            auto rotation = glm::normalize(animNode->rotationsKeys[0].rotation);
            return glm::toMat4(rotation);
        }
        
        int p0 = getRotationIndex(animationTime, animNode);
        int p1 = p0 + 1;
        float scaleFactor = getScaleFactor(animNode->rotationsKeys[p0].time, animNode->rotationsKeys[p1].time, animationTime);
        Quaternion finalRot = glm::slerp(animNode->rotationsKeys[p0].rotation, animNode->rotationsKeys[p1].rotation, scaleFactor);
        finalRot = glm::normalize(finalRot);
        
        return glm::toMat4(finalRot);
    }

    Mat4 Model::interpolateScaling(float animationTime, AnimNode* animNode)
    {
        if (animNode->scalingsKeys.size() == 1)
        {
            return glm::scale(Mat4(1.0f), animNode->scalingsKeys[0].scale);
        }

        int p0 = getScaleIndex(animationTime, animNode);
        int p1 = p0 + 1;
        float scaleFactor = getScaleFactor(animNode->scalingsKeys[p0].time, animNode->scalingsKeys[p1].time, animationTime);

        Vec3 finalScale = glm::mix(animNode->scalingsKeys[p0].scale, animNode->scalingsKeys[p1].scale, scaleFactor);

        return glm::scale(Mat4(1.0f), finalScale);
    }

    float Model::getScaleFactor(float previousTime, float nextTime, float animationTime) const
    {
        float midWayLength = animationTime - previousTime;
        float framesDiff = nextTime - previousTime;

        float scaleFactor = midWayLength / framesDiff;
        return scaleFactor;
    }

    int Model::getPositionIndex(float animationTime, AnimNode* animNode) const
    {
        int numPos = animNode->positionsKeys.size();
        for (int i = 0; i < numPos-1; i++)
        {
            if (animationTime < (float)animNode->positionsKeys[i+1].time)
            {
                return i;
            }
        }
        assert(0);
    }

    int Model::getRotationIndex(float animationTime, AnimNode* animNode) const
    {
        int numRot = animNode->rotationsKeys.size();
        for (int i = 0; i < numRot-1; i++)
        {
            if (animationTime < (float)animNode->rotationsKeys[i+1].time)
            {
                return i;
            }
        }
        assert(0);
    }

    int Model::getScaleIndex(float animationTime, AnimNode* animNode) const
    {
        int numScal = animNode->scalingsKeys.size();
        for (int i = 0; i < numScal-1; i++)
        {
            if (animationTime < (float)animNode->scalingsKeys[i+1].time)
            {
                return i;
            }
        }
        assert(0);
    }

}