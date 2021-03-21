#ifndef FILEUTILS_H
#define FILEUTILS_H

#include "Core/core.h"
#include <cstring>

namespace ningen {

    static std::string readFile(const char* filepath)
    {
        FILE* file = fopen(filepath, "rt");
        if (file == NULL)
        {
            LOG_ERROR("Failed to open a file!");
            exit(1);
        }
        
        LOG_INFO("Opening file: ", filepath);

        fseek(file, 0, SEEK_END);
        unsigned long length = ftell(file);
        fseek(file, 0, SEEK_SET);

        char* buffer = (char*)malloc(length + 1);

        memset(buffer, 0, length+1);

        fread(buffer, sizeof(char), length, file);

        std::string result(buffer);

        fclose(file);
        free(buffer);

        return result;
    }
}

#endif