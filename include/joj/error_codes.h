#ifndef JOJ_ERROR_CODES_H
#define JOJ_ERROR_CODES_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

namespace joj
{
    enum class ErrorCode : i32
    {
        WARNING = 1,
        OK = 0,
        UNKNOWN = -1,
        FAILED = -2,

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
}

#endif // JOJ_ERROR_CODES_H
