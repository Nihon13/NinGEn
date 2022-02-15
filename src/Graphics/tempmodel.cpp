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
        m_Scene = importer.ReadFile(m_Path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);

        if (!m_Scene)
        {
            LOG_ERROR(importer.GetErrorString());
            return false;
        }
        
        m_NumMeshes = m_Scene->mNumMeshes;
        
        for (unsigned int i = 0; i < m_NumMeshes; i++)
        {
            extractBonesNames(m_Scene->mMeshes[i]);
            extractMeshData(m_Scene->mMeshes[i]);
        }

        readNodesHierarchy(m_Scene->mRootNode, -1, m_RootFinalTransformation);
        m_FinalBonesMatrices.resize(m_Bones.size());

        for (unsigned int i = 0; i < m_NumMeshes; i++)
        {
            extractBoneWeights(m_Scene->mMeshes[i], m_Vertices[i]);
            m_Meshes.push_back(new Mesh(m_Vertices[i], m_Indices[i]));
        }

        getBoneTransform();

        return true;
    }

    void TempModel::extractBonesNames(const aiMesh* mesh)
    {
        unsigned int numBones = mesh->mNumBones;

        for (unsigned int i = 0; i < numBones; i++)
        {
            addBoneToList(mesh->mBones[i]);
        }
    }

    void TempModel::addBoneToList(const aiBone* bone)
    {
        std::string boneName = bone->mName.C_Str();

        int result = findBoneId(boneName);

        if (result == -1)
        {
            m_BonesNamesList.push_back(boneName);
        }
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

    void TempModel::readNodesHierarchy(const aiNode* node, int parentID, Mat4& parentTransformation)
    {
        std::string nodeName = node->mName.C_Str();

        Mat4 nodeTransformation = AssimpToGLM(node->mTransformation);

        Mat4 globalTransformation = parentTransformation * nodeTransformation;

        auto result = std::find(m_BonesNamesList.begin(), m_BonesNamesList.end(), nodeName);
        
        if (result != m_BonesNamesList.end())
        {
            Bone boneBuffer;
            boneBuffer.id = m_Bones.size();
            boneBuffer.name = nodeName;
            boneBuffer.offsetMatrix = Mat4(1.0f);
            boneBuffer.transformMatrix = AssimpToGLM(node->mTransformation);
            boneBuffer.parentBoneID = parentID;
            parentID = m_Bones.size();
            m_Bones.push_back(boneBuffer);
        }

        if (m_Bones.size() == 1)
        {
            m_RootFinalTransformation = parentTransformation;
        }

        for (int i = 0; i < node->mNumChildren; i++)
        {
            readNodesHierarchy(node->mChildren[i], parentID, globalTransformation);
        }
    }

    void TempModel::extractBoneWeights(const aiMesh* mesh, std::vector<Vertex>& vertices)
    {
        unsigned int numBones = mesh->mNumBones;

        for (unsigned int i = 0; i < numBones; i++)
        {
            aiBone* bone = mesh->mBones[i];
            std::string boneName = bone->mName.C_Str();
            unsigned int numWeights = bone->mNumWeights;
            int index = findBoneId(boneName);
            
            m_Bones[index].offsetMatrix = AssimpToGLM(bone->mOffsetMatrix);

            if (numWeights > 0)
            {
                for (unsigned int j = 0; j < numWeights; j++)
                {
                    for (unsigned int boneInfluence = 0; boneInfluence < MAX_BONE_INFLUENCE; boneInfluence++)
                    {
                        if (vertices[bone->mWeights[j].mVertexId].boneIDs[boneInfluence] == -1)
                        {
                            vertices[bone->mWeights[j].mVertexId].boneIDs[boneInfluence] = m_Bones[index].id;
                            vertices[bone->mWeights[j].mVertexId].weights[boneInfluence] = bone->mWeights[j].mWeight;
                            break;
                        }
                    }
                }
            }
        }
    }

    int TempModel::findBoneId(std::string name)
    {
        auto result = std::find(m_BonesNamesList.begin(), m_BonesNamesList.end(), name);

        if (result != m_BonesNamesList.end())
        {
            return result - m_BonesNamesList.begin();
        }

        return -1;
    }

    void TempModel::getBoneTransform(void)
    {
        for (int i = 0; i < m_Bones.size(); i++)
        {
            if (m_Bones[i].parentBoneID == -1)
            {
                m_FinalBonesMatrices[i] = m_RootFinalTransformation * m_Bones[i].transformMatrix;
            }
            else
            {
                m_FinalBonesMatrices[i] = m_FinalBonesMatrices[m_Bones[i].parentBoneID] * m_Bones[i].transformMatrix;
            }
        }

        for (int i = 0; i < m_Bones.size(); i++)
        {
            m_FinalBonesMatrices[i] = m_FinalBonesMatrices[i] * m_Bones[i].offsetMatrix;
        }
    }
}