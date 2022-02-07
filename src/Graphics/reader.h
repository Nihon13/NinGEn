#ifndef READER_H
#define READER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <map>
#include <algorithm>
#include "nhntxtcol.h"
#include <Math/math.h>

#define LOG_INFO(...) nhntxtcol::TextColor::WHT(__VA_ARGS__)
#define LOG_TRACE(...) nhntxtcol::TextColor::GRN(__VA_ARGS__)
#define LOG_WARN(...) nhntxtcol::TextColor::YLW(__VA_ARGS__)
#define LOG_ERROR(...) nhntxtcol::TextColor::RED(__VA_ARGS__)

#define MAX_BONE_INFLUENCE 3

struct Vertex
{
    float position[3];
    float normal[3];
    float uv[2];
    float bitangent[3];
    float tangent[3];
    int boneIDs[MAX_BONE_INFLUENCE];
    float weights[MAX_BONE_INFLUENCE];
};

struct Bone
{
    int id;
    std::string name;
    Mat4 offsetMatrix;
    Mat4 transformMatrix;
    int parentBoneID;

    bool operator<(const Bone& a) const
    {
        return id < a.id;
    }
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

class Reader
{
    private:
        const char* m_Path;
        unsigned int m_NumMeshes = 0;
        unsigned int m_VerticesTotalCount = 0;
        unsigned int m_IndicesTotalCount = 0;
        unsigned int m_BoneCounter = 0;
        unsigned int m_NumAnimations = 0;
        std::map<std::string, Bone> m_BonesMap;

        bool init(void);
        void processMesh(const aiMesh* mesh);
        void extractBoneInfo(const aiMesh* mesh, std::vector<Vertex>& vertices);
        void addBoneToList(const aiBone* bone);
        int findParentBone(Bone& bone) const;
        void extractAnimInfo(const aiAnimation* anim);
    public:
        const aiScene* m_Scene;
        std::vector<std::vector<Vertex>> m_Vertices;
        std::vector<std::vector<unsigned int>> m_Indices;
        std::vector<Bone> m_Bones;
        std::vector<Animation> m_Animations;
        
        Reader(const char* path);
        
        inline unsigned int getNumMeshes(void) const { return m_NumMeshes; }
        inline unsigned int getNumAnimations(void) const { return m_NumAnimations; }
        inline unsigned int getVerticesTotalCount(void) const { return m_VerticesTotalCount; }
        inline unsigned int getIndicesTotalCount(void) const { return m_IndicesTotalCount; }

        static Mat4 AssimpToGLM(const aiMatrix4x4 &matrix)
        {
            return Mat4(
                matrix[0][0], matrix[1][0], matrix[2][0], matrix[3][0],
                matrix[0][1], matrix[1][1], matrix[2][1], matrix[3][1],
                matrix[0][2], matrix[1][2], matrix[2][2], matrix[3][2],
                matrix[0][3], matrix[1][3], matrix[2][3], matrix[3][3]
            );
        }

        void matToString(aiMatrix4x4 mat)
        {
            LOG_TRACE("mat4x4((", mat.a1, ", ", mat.a2, ", ", mat.a3, ", ", mat.a4, "), (", mat.b1, ", ", mat.b2, ", ", mat.b3, ", ", mat.b4, "), (", mat.c1, ", ", mat.c2, ", ", mat.c3, ", ", mat.c4, "), (", mat.d1, ", ", mat.d2, ", ", mat.d3, ", ", mat.d4, "))");
        }

};

#endif