#include "tempmodel.h"

namespace ningen {

    TempModel::TempModel(const char* path)
        : m_Path(path)
    {
        if (!initAssimp())
        {
            exit(EXIT_FAILURE);
        }
        else
        {
            LOG_INFO("Loaded TempModel: [", m_Path, "]");
        }
    }

    TempModel::~TempModel(void)
    {
        for (int i = 0; i < m_Meshes.size(); i++)
        {
            delete m_Meshes[i];
        }
    }

    bool TempModel::initAssimp(void)
    {
        Assimp::Importer importer;
        m_Scene = importer.ReadFile(m_Path, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);

        if (!m_Scene)
        {
            LOG_ERROR(importer.GetErrorString());
            return false;
        }
        
        m_NumMeshes = m_Scene->mNumMeshes;
        
        for (unsigned int i = 0; i < m_NumMeshes; i++)
        {
            extractMeshData(m_Scene->mMeshes[i]);
        }

        return true;
    }

    void TempModel::extractMeshData(const aiMesh* mesh)
    {
        //////// VERTICES //////////////////////////////////////////
        unsigned int numVertices = mesh->mNumVertices;

        std::vector<Vertex> vertices;

        Vertex vertexBuff;

        for (unsigned int i = 0; i < numVertices; i++)
        {
            vertexBuff.position[0] = mesh->mVertices[i].x;
            vertexBuff.position[1] = mesh->mVertices[i].y;
            vertexBuff.position[2] = mesh->mVertices[i].z;

            vertexBuff.normal[0] = mesh->mNormals[i].x;
            vertexBuff.normal[1] = mesh->mNormals[i].y;
            vertexBuff.normal[2] = mesh->mNormals[i].z;

            vertexBuff.uv[0] = mesh->mTextureCoords[0][i].x;
            vertexBuff.uv[1] = mesh->mTextureCoords[0][i].y;

            vertexBuff.bitangent[0] = mesh->mBitangents[i].x;
            vertexBuff.bitangent[1] = mesh->mBitangents[i].y;
            vertexBuff.bitangent[2] = mesh->mBitangents[i].z;

            vertexBuff.tangent[0] = mesh->mTangents[i].x;
            vertexBuff.tangent[1] = mesh->mTangents[i].y;
            vertexBuff.tangent[2] = mesh->mTangents[i].z;

            for (unsigned int j = 0; j < MAX_BONE_INFLUENCE; j++)
            {
                vertexBuff.boneIDs[j] = -1;
                vertexBuff.weights[j] = 0.0f;
            }

            vertices.push_back(vertexBuff);
        }

        m_Vertices.push_back(vertices);

        //////// INDICES ///////////////////////////////////////////
        std::vector<unsigned int> indices;

        unsigned int numFaces = mesh->mNumFaces;

        for (unsigned int i = 0; i < numFaces; i++)
        {
            aiFace face = mesh->mFaces[i];

            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        m_Indices.push_back(indices);
    }

}