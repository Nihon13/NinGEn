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

        for (int i = 0; i < m_Scene->mNumAnimations; i++)
        {
            extractAnimInfo(m_Scene->mAnimations[i]);
        }

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

        auto result = std::find(m_BonesNamesList.begin(), m_BonesNamesList.end(), boneName);

        if (result == m_BonesNamesList.end())
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
        for (int i = 0; i < m_Bones.size(); i++)
        {
            if (m_Bones[i].name == name)
            {
                return i;
            }
        }

        return -1;
    }

    void TempModel::getBoneTransform(float timeInSec)
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
                Mat4 translate = Mat4(1.0f);
                // LOG_TRACE(animNode->positionsKeys.size());
                translate = glm::translate(translate, Vec3(animNode->positionsKeys[(int)animationTimeTicks].position[0], animNode->positionsKeys[(int)animationTimeTicks].position[1], animNode->positionsKeys[(int)animationTimeTicks].position[2]));
                translate = interpolatePosition(animationTimeTicks, animNode);
                glm::quat Quat = glm::quat(animNode->rotationsKeys[(int)animationTimeTicks].rotation[0], animNode->rotationsKeys[(int)animationTimeTicks].rotation[1], animNode->rotationsKeys[(int)animationTimeTicks].rotation[2], animNode->rotationsKeys[(int)animationTimeTicks].rotation[3]);
                Mat4 rotate = toMat4(Quat);
                rotate = interpolateRotation(animationTimeTicks, animNode);
                Mat4 scale = Mat4(1.0f);
                scale = interpolateScaling(animationTimeTicks, animNode);
                boneTransform = translate * rotate * scale;
                // LOG_TRACE(animNode->positionsKeys[(int)animationTimeTicks].time, " | ", timeInSec);
                // boneTransform = animNode.positionKeys[(int)animationTimeTicks] * animNode.rotationKeys[(int)animationTimeTicks]
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

    void TempModel::extractAnimInfo(const aiAnimation* anim)
    {
        Animation animation;

        animation.name = anim->mName.C_Str();
        animation.duration = anim->mDuration;
        animation.ticksPerSecond = anim->mTicksPerSecond;

        for (int i = 0; i < anim->mNumChannels; i++)
        {
            aiNodeAnim* aiNode = anim->mChannels[i];
            AnimNode animNode;
            std::string name = aiNode->mNodeName.C_Str();

            animNode.id = m_Bones[findBoneId(name)].id;

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

    int TempModel::findAnimBone(int animID, int boneID) const
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

    float TempModel::getScaleFactor(float previousTime, float nextTime, float animationTime)
    {
        float midWayLength = animationTime - previousTime;
        float framesDiff = nextTime - previousTime;

        float scaleFactor = midWayLength / framesDiff;
        return scaleFactor;
    }

    Mat4 TempModel::interpolatePosition(float animationTime, AnimNode* animNode)
    {
        if (animNode->positionsKeys.size() == 1)
        {
            // return glm::translate(Mat4(1.0f), animNode->positionsKeys[0].position); // TODO
            return glm::translate(Mat4(1.0f), Vec3(animNode->positionsKeys[0].position[0], animNode->positionsKeys[0].position[1], animNode->positionsKeys[0].position[2]));
        }

        int p0 = getPositionIndex(animationTime, animNode);
        int p1 = p0 + 1;
        float scaleFactor = getScaleFactor(animNode->positionsKeys[p0].time, animNode->positionsKeys[p1].time, animationTime);
        Vec3 pos0 = Vec3(animNode->positionsKeys[p0].position[0], animNode->positionsKeys[p0].position[1], animNode->positionsKeys[p0].position[2]);
        Vec3 pos1 = Vec3(animNode->positionsKeys[p1].position[0], animNode->positionsKeys[p1].position[1], animNode->positionsKeys[p1].position[2]);
        Vec3 finalPos = glm::mix(pos0, pos1, scaleFactor);

        return glm::translate(Mat4(1.0f), finalPos);
    }

    Mat4 TempModel::interpolateRotation(float animationTime, AnimNode* animNode)
    {
        if (animNode->rotationsKeys.size() == 1)
        {
            auto rotation = glm::normalize(Quaternion(animNode->rotationsKeys[0].rotation[0], animNode->rotationsKeys[0].rotation[1], animNode->rotationsKeys[0].rotation[2], animNode->rotationsKeys[0].rotation[3]));
            return glm::toMat4(rotation);
        }
        
        int p0 = getRotationIndex(animationTime, animNode);
        int p1 = p0 + 1;
        float scaleFactor = getScaleFactor(animNode->rotationsKeys[p0].time, animNode->rotationsKeys[p1].time, animationTime);
        Quaternion rot0 = Quaternion(Quaternion(animNode->rotationsKeys[p0].rotation[0], animNode->rotationsKeys[p0].rotation[1], animNode->rotationsKeys[p0].rotation[2], animNode->rotationsKeys[p0].rotation[3]));
        Quaternion rot1 = Quaternion(Quaternion(animNode->rotationsKeys[p1].rotation[0], animNode->rotationsKeys[p1].rotation[1], animNode->rotationsKeys[p1].rotation[2], animNode->rotationsKeys[p1].rotation[3]));
        Quaternion finalRot = glm::slerp(rot0, rot1, scaleFactor);
        finalRot = glm::normalize(finalRot);
        
        return glm::toMat4(finalRot);
    }

    Mat4 TempModel::interpolateScaling(float animationTime, AnimNode* animNode)
    {
        if (animNode->scalingsKeys.size() == 1)
        {
            return glm::scale(Mat4(1.0f), Vec3(animNode->scalingsKeys[0].scale[0], animNode->scalingsKeys[0].scale[1], animNode->scalingsKeys[0].scale[2]));
        }

        int p0 = getScaleIndex(animationTime, animNode);
        int p1 = p0 + 1;
        float scaleFactor = getScaleFactor(animNode->scalingsKeys[p0].time, animNode->scalingsKeys[p1].time, animationTime);
        Vec3 scal0 = Vec3(animNode->scalingsKeys[p0].scale[0], animNode->scalingsKeys[p0].scale[1], animNode->scalingsKeys[p0].scale[2]);
        Vec3 scal1 = Vec3(animNode->scalingsKeys[p1].scale[0], animNode->scalingsKeys[p1].scale[1], animNode->scalingsKeys[p1].scale[2]);
        Vec3 finalScale = glm::mix(scal0, scal1, scaleFactor);

        return glm::scale(Mat4(1.0f), finalScale);
    }

    int TempModel::getPositionIndex(float animationTime, AnimNode* animNode)
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

    int TempModel::getRotationIndex(float animationTime, AnimNode* animNode)
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

    int TempModel::getScaleIndex(float animationTime, AnimNode* animNode)
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