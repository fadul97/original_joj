#include "error.h"

const char* joj::error_to_string(joj::ErrorCode e)
{
    switch (e)
    {
        case ErrorCode::OK: return "OK";
        case ErrorCode::FAILED: return "FAILED";
        case ErrorCode::ERR_WINDOW_INIT: return "ERR_WINDOW_INIT";
        case ErrorCode::ERR_WINDOW_CREATION: return "ERR_WINDOW_CREATION";
        case ErrorCode::ERR_WINDOW_ID_CREATION: return "ERR_WINDOW_ID_CREATION";
        case ErrorCode::ERR_WINDOW_DISPLAY: return "ERR_WINDOW_DISPLAY";
        case ErrorCode::ERR_CONTEXT_INIT: return "ERR_CONTEXT_INIT";
        case ErrorCode::ERR_CONTEXT_CREATION: return "ERR_CONTEXT_CREATION";
        case ErrorCode::ERR_CONTEXT_GLX_VERSION: return "ERR_CONTEXT_GLX_VERSION";
        case ErrorCode::ERR_CONTEXT_FB_UNMATCH: return "ERR_CONTEXT_FB_UNMATCH";
        case ErrorCode::ERR_CONTEXT_VISUAL_CREATION: return "ERR_CONTEXT_VISUAL_CREATION";
        case ErrorCode::ERR_CONTEXT_SCREEN_UNMATCH: return "ERR_CONTEXT_SCREEN_UNMATCH";
        case ErrorCode::ERR_CONTEXT_EXTENSION_STR: return "ERR_CONTEXT_EXTENSION_STR";
        default: return "UNKOWN";
    }
}
