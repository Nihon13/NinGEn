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

        for (int i = 0; i < bonesCount; i++)
        {
            Bone bone;
            size_t size;
            char* buff = new char[size];

            file.read((char*)&bone.id, sizeof(((Bone*)0)->id));
            file.read((char*)&size, sizeof(size));
            file.read(buff, size);
            bone.name = buff;
            delete [] buff;
            file.read((char*)&bone.offsetMatrix, sizeof(((Bone*)0)->offsetMatrix));
            file.read((char*)&bone.transformMatrix, sizeof(((Bone*)0)->transformMatrix));
            file.read((char*)&bone.parentBoneID, sizeof(((Bone*)0)->parentBoneID));

            m_Bones.push_back(bone);            
        }

        file.close();

        return true;
    }

}