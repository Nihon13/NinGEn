#include "reader.h"

using namespace ningen;

Reader::Reader(const char* path)
    : m_Path(path)
{
    if (!init())
    {
        exit(EXIT_FAILURE);
    }
    else
    {
        LOG_INFO("Loaded file: ", m_Path);
    }
}

bool Reader::init(void)
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
        processMesh(m_Scene->mMeshes[i]);
        m_VerticesTotalCount += m_Vertices[i].size();
        m_IndicesTotalCount += m_Indices[i].size();
    }

    for (auto iter = m_BonesMap.begin(); iter != m_BonesMap.end(); iter++)
    {
        Bone bone;
        bone.id = iter->second.id;
        bone.name = iter->first;
        bone.offsetMatrix = iter->second.offsetMatrix;
        // bone.offsetMatrix.Transpose();
        bone.parentBoneID = -1;

        m_Bones.push_back(bone);
    }

    std::sort(m_Bones.begin(), m_Bones.end());

    for (int i = 0; i < m_Bones.size(); i++)
    {      
        m_Bones[i].parentBoneID = findParentBone(m_Bones[i]);
        m_Bones[i].transformMatrix = AssimpToGLM(m_Scene->mRootNode->FindNode(m_Bones[i].name.c_str())->mTransformation);
        // aiMatrix4x4 mat = m_Bones[i].transformMatrix;
        // LOG_TRACE(glm::to_string(m_Bones[i].transformMatrix)); 
        // LOG_TRACE("mat4x4((", mat.a1, ", ", mat.a2, ", ", mat.a3, ", ", mat.a4, "), (", mat.b1, ", ", mat.b2, ", ", mat.b3, ", ", mat.b4, "), (", mat.c1, ", ", mat.c2, ", ", mat.c3, ", ", mat.c4, "), (", mat.d1, ", ", mat.d2, ", ", mat.d3, ", ", mat.d4, "))");
        // m_Bones[i].transformMatrix.Transpose();
    }

    m_NumAnimations = m_Scene->mNumAnimations;

    for (int i = 0; i < m_NumAnimations; i++)
    {
        extractAnimInfo(m_Scene->mAnimations[i]);
    }

    return true;
}

void Reader::processMesh(const aiMesh* mesh)
{
    unsigned int numVertices = mesh->mNumVertices;

    std::vector<Vertex> vertices;

    Vertex buffer;

    for (unsigned int i = 0; i < numVertices; i++)
    {
        buffer.position[0] = mesh->mVertices[i].x;
        buffer.position[1] = mesh->mVertices[i].y;
        buffer.position[2] = mesh->mVertices[i].z;

        buffer.normal[0] = mesh->mNormals[i].x;
        buffer.normal[1] = mesh->mNormals[i].y;
        buffer.normal[2] = mesh->mNormals[i].z;

        buffer.uv[0] = mesh->mTextureCoords[0][i].x;
        buffer.uv[1] = mesh->mTextureCoords[0][i].y;

        buffer.bitangent[0] = mesh->mBitangents[i].x;
        buffer.bitangent[1] = mesh->mBitangents[i].y;
        buffer.bitangent[2] = mesh->mBitangents[i].z;

        buffer.tangent[0] = mesh->mTangents[i].x;
        buffer.tangent[1] = mesh->mTangents[i].y;
        buffer.tangent[2] = mesh->mTangents[i].z;

        for (unsigned int j = 0; j < MAX_BONE_INFLUENCE; j++)
        {
            buffer.boneIDs[j] = -1;
            buffer.weights[j] = 0.0f;
        }

        vertices.push_back(buffer);
    }

    extractBoneInfo(mesh, vertices);
    
    m_Vertices.push_back(vertices);

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

void Reader::extractBoneInfo(const aiMesh* mesh, std::vector<Vertex>& vertices)
{
    unsigned int numBones = mesh->mNumBones;

    for (unsigned int i = 0; i < numBones; i++)
    {
        aiBone* bone = mesh->mBones[i];
        unsigned int numWeights = bone->mNumWeights;
        
        addBoneToList(bone);
        
        if (numWeights > 0)
        {
            for (unsigned int j = 0; j < numWeights; j++)
            {
                for (unsigned int boneInfluence = 0; boneInfluence < MAX_BONE_INFLUENCE; boneInfluence++)
                {
                    if (vertices[bone->mWeights[j].mVertexId].boneIDs[boneInfluence] == -1)
                    {
                        vertices[bone->mWeights[j].mVertexId].boneIDs[boneInfluence] = m_BonesMap.at(bone->mName.C_Str()).id;
                        vertices[bone->mWeights[j].mVertexId].weights[boneInfluence] = bone->mWeights[j].mWeight;
                        break;
                    }
                }
            }
        }
    }
}

void Reader::addBoneToList(const aiBone* bone)
{
    std::string boneName = bone->mName.C_Str();

    if (m_BonesMap.find(boneName) == m_BonesMap.end())
    {
        m_BonesMap[boneName].id = m_BoneCounter;
        matToString(bone->mOffsetMatrix);
        m_BonesMap[boneName].offsetMatrix = AssimpToGLM(bone->mOffsetMatrix);
        // LOG_TRACE("Macierz: ", glm::to_string(m_BonesMap[boneName].offsetMatrix));
        m_BoneCounter++;
    }
}

int Reader::findParentBone(Bone& bone) const
{    
    aiNode* node = m_Scene->mRootNode->FindNode(bone.name.c_str());
    
    std::string parentName;
    if (node != nullptr)
    {
        parentName = node->mParent->mName.C_Str();
        if (m_BonesMap.find(parentName) != m_BonesMap.end())
        {
            return m_BonesMap.at(parentName).id;
        }
    }

    return -1;
}

void Reader::extractAnimInfo(const aiAnimation* anim)
{
    Animation animation;

    animation.name = anim->mName.C_Str();
    animation.duration = anim->mDuration;
    animation.ticksPerSecond = anim->mTicksPerSecond;

    for (int i = 0; i < anim->mNumChannels; i++)
    {
        aiNodeAnim* aiNode = anim->mChannels[i];
        AnimNode animNode;

        animNode.id = m_BonesMap.at(aiNode->mNodeName.C_Str()).id;

        for (int j = 0; j < aiNode->mNumPositionKeys; j++)
        {
            KeyPos kp;
            kp.position[0] = aiNode->mPositionKeys[j].mValue.x;
            kp.position[1] = aiNode->mPositionKeys[j].mValue.y;
            kp.position[2] = aiNode->mPositionKeys[j].mValue.z;
            kp.time = aiNode->mPositionKeys[j].mTime;

            animNode.positionsKeys.push_back(kp);
        }

        for (int j = 0; j < aiNode->mNumRotationKeys; j++)
        {
            KeyRot kr;
            kr.rotation[0] = aiNode->mRotationKeys[j].mValue.w;
            kr.rotation[1] = aiNode->mRotationKeys[j].mValue.x;
            kr.rotation[2] = aiNode->mRotationKeys[j].mValue.y;
            kr.rotation[3] = aiNode->mRotationKeys[j].mValue.z;
            kr.time = aiNode->mRotationKeys[j].mTime;

            animNode.rotationsKeys.push_back(kr);
        }

        for (int j = 0; j < aiNode->mNumRotationKeys; j++)
        {
            KeyScale ks;
            ks.scale[0] = aiNode->mScalingKeys[j].mValue.x;
            ks.scale[1] = aiNode->mScalingKeys[j].mValue.y;
            ks.scale[2] = aiNode->mScalingKeys[j].mValue.z;
            ks.time = aiNode->mScalingKeys[j].mTime;

            animNode.scalingsKeys.push_back(ks);
        }

        animation.channels.push_back(animNode);
    }

    m_Animations.push_back(animation);
}