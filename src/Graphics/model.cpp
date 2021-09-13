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
        m_SkeleAnimManager.m_BonesCount = bonesCount;

        if (bonesCount > 0)
        {
            for (int i = 0; i < bonesCount; i++)
            {
                m_SkeleAnimManager.m_FinalBonesMatrices.push_back(Mat4(1.0f));

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

                m_SkeleAnimManager.m_Bones.push_back(bone);
            }

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
                    size_t size;
                    file.read((char*)&size, sizeof(size));
                    char* buff = new char[size];
                    file.read(buff, size);
                    node.name = buff;
                    delete [] buff;

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

                m_SkeleAnimManager.m_Animations.push_back(animation);
            }
        }

        file.close();

        return true;
    }

}