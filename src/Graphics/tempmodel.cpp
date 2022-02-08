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
            processMesh(m_Scene->mMeshes[i]);
        }

        return true;
    }

    void TempModel::processMesh(const aiMesh* mesh)
    {

    }

}