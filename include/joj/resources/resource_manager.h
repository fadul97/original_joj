#ifndef JOJ_RESOURCE_MANAGER_H
#define JOJ_RESOURCE_MANAGER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "error_code.h"
#include <string>

namespace joj {
    struct Shader {
        std::string vshader_code;
        std::string fshader_code;
    };
}

namespace joj::ResourceManager {
    JAPI ErrorCode create_and_build_glshader(Shader& shader, const char* vertex_path, const char* fragment_path);
}


#endif // JOJ_RESOURCE_MANAGER_H
