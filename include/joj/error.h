#ifndef JOJ_ERROR_H
#define JOJ_ERROR_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <string>

namespace joj
{
    enum class ErrorCode
    {
        UNKNOWN,
        OK,
        FAILED,

        // General Engine error codes
        ERR_ENGINE_INIT,

        // General PlatformManager error codes
        ERR_PLATFORM_MANAGER_INIT,
        ERR_PLATFORM_MANAGER_CREATION,
        
        // General Window error codes
        ERR_WINDOW_INIT,
        ERR_WINDOW_CREATION,
        ERR_WINDOW_ID_CREATION,

        // General Input error codes
        ERR_INPUT_CREATION,

        // General Timer error codes
        ERR_TIMER_CREATION,

        // X11Window error codes
        ERR_WINDOW_DISPLAY,

        // TODO: Win32Window error codes

        // General Renderer error codes
        ERR_RENDERER_INIT,
        ERR_RENDERER_PIPELINE_ERROR,
        
        // General GraphicsContext error codes
        ERR_CONTEXT_INIT,
        ERR_CONTEXT_CREATION,
        
        // X11GLContext error codes
        ERR_CONTEXT_GLX_VERSION,
        ERR_CONTEXT_FB_UNMATCH,
        ERR_CONTEXT_VISUAL_CREATION,
        ERR_CONTEXT_SCREEN_UNMATCH,
        ERR_CONTEXT_EXTENSION_STR,

        // X11Renderer error codes
        ERR_RENDERER_SWAPCHAIN_CREATION,
        ERR_RENDERER_SWAPCHAIN_GET_BUFFER,
        ERR_RENDER_TARGET_VIEW_CREATION,
        ERR_RENDERER_TEXTURE2D_CREATION,
        ERR_RENDERER_DEPTHSTENCIL_VIEW_CREATION,
        ERR_RENDERER_BLEND_STATE_CREATION,
        ERR_RENDERER_RASTERIZER_CREATION
    };

    void print_error(ErrorCode e, const std::string& func, const std::string& file, i32 line);

    const char* error_to_string(ErrorCode e);

    class JAPI Error
    {
    public:
        Error();
        Error(ErrorCode e, const std::string& func, const std::string& file, i32 line);
        ~Error();

        std::string to_string() const;
        void what() const;

    private:
        ErrorCode m_ecode;
        std::string m_func_name;
        std::string m_filename;
        i32 m_line;
    };
}

#define JFAILED(x) (((joj::ErrorCode)(x)) != joj::ErrorCode::OK)

#ifndef JOUTPUTFAILED
#define JOUTPUTFAILED(x)                                                                                          \
{                                                                                                                 \
    joj::ErrorCode result = (x);                                                                                  \
    if(result != joj::ErrorCode::OK) { joj::print_error(result, __func__, __FILE__, __LINE__); return result; }   \
}
#endif

#endif // JOJ_ERROR_H
