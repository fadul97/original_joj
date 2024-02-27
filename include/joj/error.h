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
        ERR_WINDOW_CREATION,
        ERR_WINDOW_ID_CREATION,

        // X11Window error codes
        ERR_WINDOW_DISPLAY,

        // TODO: Win32Window error codes
        
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
        ERR_RENDERER_SWAPCHAIN_CREATE,
        ERR_RENDERER_SWAPCHAIN_GET_BUFFER,
        ERR_RENDER_TARGET_VIEW_CREATION,
        ERR_RENDERER_TEXTURE2D_CREATION,
        ERR_RENDERER_DEPTHSTENCIL_VIEW_CREATION,
        ERR_RENDERER_BLEND_STATE_CREATION,
        ERR_RENDERER_RASTERIZER_CREATION
    };

    const char* error_to_string(ErrorCode e);
}

#endif // JOJ_ERROR_H
