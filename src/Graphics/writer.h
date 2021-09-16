#ifndef WRITER_H
#define WRITER_H

#include "reader.h"

#define HEADER "NHMF"
#define VERSION "0005"
#define FOOTER "1990"

void matToString(aiMatrix4x4 mat)
{
    LOG_TRACE("mat4x4((", mat.a1, ", ", mat.a2, ", ", mat.a3, ", ", mat.a4, "), (", mat.b1, ", ", mat.b2, ", ", mat.b3, ", ", mat.b4, "), (", mat.c1, ", ", mat.c2, ", ", mat.c3, ", ", mat.c4, "), (", mat.d1, ", ", mat.d2, ", ", mat.d3, ", ", mat.d4, "))");
}

void writeToFile(const Reader& reader, const char* outPath)
{
    std::ofstream file(outPath, std::ios::out | std::ios::binary);
    if (!file)
    {
        LOG_ERROR("Cannot open file!");
    }

    const char* header = HEADER;
    const char* version = VERSION;

    // write header and version
    size_t headerSize = strlen(header) + 1;
    size_t versionSize = strlen(version) + 1;

    file.write((char*)&headerSize, sizeof(headerSize));
    file.write(header, headerSize);
    file.write((char*)&versionSize, sizeof(versionSize));
    file.write(version, versionSize);

    // write meshes count
    size_t meshesCount = reader.getNumMeshes();
    file.write((char*)&meshesCount, sizeof(meshesCount));

    for (unsigned int i = 0; i < meshesCount; i++)
    {
        size_t verticesCount = reader.m_Vertices[i].size();
        size_t indicesCount = reader.m_Indices[i].size();

        LOG_INFO("Mesh ",i+1,": Vertices: ",verticesCount, " Indices: ", indicesCount);

        // write vertices and indices count
        file.write((char*)&verticesCount, sizeof(verticesCount));
        file.write((char*)&indicesCount, sizeof(indicesCount));

        // write vertices and indices
        file.write((char*)&reader.m_Vertices[i][0], verticesCount * sizeof(Vertex));
        file.write((char*)&reader.m_Indices[i][0], indicesCount * sizeof(unsigned int));
    }

    // write bones count
    size_t bonesCount = reader.m_Bones.size();
    file.write((char*)&bonesCount, sizeof(bonesCount));

    for (int i = 0; i < bonesCount; i++)
    {
        // write bones data
        file.write((char*)&reader.m_Bones[i].id, sizeof(((Bone*)0)->id));
        size_t size = reader.m_Bones[i].name.size();
        file.write((char*)&size, sizeof(size)); // write size of bone's name
        file.write(reader.m_Bones[i].name.c_str(), size);
        file.write((char*)&reader.m_Bones[i].offsetMatrix, sizeof(((Bone*)0)->offsetMatrix));
        file.write((char*)&reader.m_Bones[i].transformMatrix, sizeof(((Bone*)0)->transformMatrix));
        file.write((char*)&reader.m_Bones[i].parentBoneID, sizeof(((Bone*)0)->parentBoneID));
    }

    // write animations count
    size_t animationsCount = reader.m_Animations.size();
    file.write((char*)&animationsCount, sizeof(animationsCount));

    // write animations data
    for (int i = 0; i < animationsCount; i++)
    {
        size_t size = reader.m_Animations[i].name.size();
        file.write((char*)&size, sizeof(size)); // write size of animation's name
        file.write(reader.m_Animations[i].name.c_str(), size);
        double duration = reader.m_Animations[i].duration;
        file.write((char*)&duration, sizeof(duration));
        double ticks = reader.m_Animations[i].ticksPerSecond;
        file.write((char*)&ticks, sizeof(ticks));

        size_t numChannels = reader.m_Animations[i].channels.size();
        file.write((char*)&numChannels, sizeof(numChannels));

        for (int j = 0; j < numChannels; j++)
        {
            AnimNode node = reader.m_Animations[i].channels[j];
            size_t numPosKeys = node.positionsKeys.size();
            size_t numRotKeys = node.rotationsKeys.size();
            size_t numScaleKeys = node.scalingsKeys.size();

            file.write((char*)&node.id, sizeof(node.id));
            file.write((char*)&numPosKeys, sizeof(numPosKeys));
            file.write((char*)&numRotKeys, sizeof(numRotKeys));
            file.write((char*)&numScaleKeys, sizeof(numScaleKeys));

            for (int k = 0; k < numPosKeys; k++)
            {
                file.write((char*)&node.positionsKeys[k], sizeof(KeyPos));
            }

            for (int k = 0; k < numRotKeys; k++)
            {
                file.write((char*)&node.rotationsKeys[k], sizeof(KeyRot));
            }

            for (int k = 0; k < numScaleKeys; k++)
            {
                file.write((char*)&node.scalingsKeys[k], sizeof(KeyScale));
            }
        }
    }

    const char* footer = FOOTER;
    size_t footerSize = strlen(footer) + 1;

    // write footer
    file.write((char*)&footerSize, sizeof(footerSize));
    file.write(footer, footerSize);

    file.close();
}

void writeToFile(const char* path)
{
    Reader reader(path);
    std::string outPath = path;

    outPath = outPath.substr(0, outPath.size() - 4) + ".nhmf";

    writeToFile(reader, outPath.c_str());
}

#endif