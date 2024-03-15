#ifndef JOJ_ENGINE_H
#define JOJ_ENGINE_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "error.h"

#if JPLATFORM_LINUX  
#include "platform/x11/platform_manager_x11.h"
#else
#include "platform/win32/platform_manager_win32.h"
#include "renderer/dx11/renderer_dx11.h"
#endif 

namespace joj
{
    // FIXME: Abstract default platform_manager and renderer
    class JAPI Engine
    {
    public:
        Engine();
        ~Engine();

        ErrorCode init();
        ErrorCode run();
        void shutdown();

    private:

#if JPLATFORM_WINDOWS  
        Win32PlatformManager* platform_manager;
        DX11Renderer* renderer;
#endif // JPLATFORM_WINDOWS
    };
} // namespace joj

#endif // JOJ_ENGINE_H