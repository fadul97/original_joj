#ifndef JOJ_ERROR_H
#define JOJ_ERROR_H

namespace joj
{
    enum class ErrorCode
    {
        UNKNOWN,
        OK,
        FAILED,
        
        // General Window error codes
        ERR_WINDOW_INIT,
        ERR_WINDOW_CREATE,
        ERR_WINDOW_ID_CREATION,

        // X11Window error codes
        ERR_WINDOW_DISPLAY,

        // TODO: Win32Window error codes
        
        // General GraphicsContext error codes
        ERR_CONTEXT_INIT,
        ERR_CONTEXT_CREATE,
        
        // X11GLContext error codes
        ERR_CONTEXT_GLX_VERSION,
        ERR_CONTEXT_FB_UNMATCH,
        ERR_CONTEXT_VISUAL_CREATION,
        ERR_CONTEXT_SCREEN_UNMATCH,
        ERR_CONTEXT_EXTENSION_STR,
    };

    const char* error_to_string(ErrorCode e);
}

#endif // JOJ_ERROR_H
