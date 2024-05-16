#ifndef JOJ_ERROR_CODES_H
#define JOJ_ERROR_CODES_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

namespace  joj {
    enum class ErrorCode : i32 {
        OK = 0,
        ERR_WINDOW = -1,
        ERR_CONTEXT = -2,
        ERR_FILE_OPEN = -3,
        ERR_VSHADER = -4,
        ERR_FSHADER = -5,
        ERR_SHADER_PROGRAM = -6,
        ERR_RENDERER = -7,
    };

    i32 err_to_i32(ErrorCode err);
    const char* err_to_str(ErrorCode err);
}

#endif // JOJ_ERROR_CODES_H
