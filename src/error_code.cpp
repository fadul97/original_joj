#include "error_code.h"

i32 joj::err_to_i32(const ErrorCode err) {
    switch (err) {
        case ErrorCode::OK:
            return 0;

        case ErrorCode::ERR_WINDOW:
            return -1;

        case ErrorCode::ERR_CONTEXT:
            return -2;

        case ErrorCode::ERR_FILE_OPEN:
            return -3;

        case ErrorCode::ERR_VSHADER:
            return -4;

        case ErrorCode::ERR_FSHADER:
            return -5;

        case ErrorCode::ERR_SHADER_PROGRAM:
            return -6;

        case ErrorCode::ERR_RENDERER:
            return -7;

        default:
            return 1;
    }
}

const char* joj::err_to_str(const ErrorCode err) {
    switch (err) {
        case ErrorCode::OK:
            return "OK";

        case ErrorCode::ERR_WINDOW:
            return "ERR_WINDOW";

        case ErrorCode::ERR_CONTEXT:
            return "ERR_CONTEXT";

        case ErrorCode::ERR_FILE_OPEN:
            return "ERR_FILE_OPEN";

        case ErrorCode::ERR_VSHADER:
            return "ERR_VSHADER";

        case ErrorCode::ERR_FSHADER:
            return "ERR_FSHADER";

        case ErrorCode::ERR_SHADER_PROGRAM:
            return "ERR_SHADER_PROGRAM";

        case ErrorCode::ERR_RENDERER:
            return "ERR_RENDERER";

        default:
            return "UNKNOWN";
    }
}