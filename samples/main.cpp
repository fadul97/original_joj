// FIXME: Main file doesn't need to define JOJ_ENGINE_IMPLEMENTATION
#define JOJ_ENGINE_IMPLEMENTATION
#include "joj/defines.h"

#include <iostream>

#if JPLATFORM_LINUX  
#include "joj/platform/x11/platform_manager_x11.h"
#include "joj/renderer/opengl/renderer_gl.h"
#elif JPLATFORM_WINDOWS
#include <memory>
#include "joj/platform/win32/window_win32.h"
#include "joj/platform/win32/input_win32.h"
#include "joj/platform/win32/timer_win32.h"
#include "joj/platform/win32/platform_manager_win32.h"
#include "joj/graphics/opengl/win32/context_gl.h"
#include "joj/graphics/dx11/context_dx11.h"
#include "joj/renderer/dx11/renderer_dx11.h"
#endif

int main()
{
#if JPLATFORM_LINUX  
    auto pm = new joj::X11PlatformManager();
#else
    auto pm = new joj::Win32PlatformManager();
#endif 
    pm->init(800, 600);
    if (!pm->create_window())
    {
        std::cout << "Failed to create window.\n";
        return -1;
    }
    
#if JPLATFORM_LINUX  
    joj::GLRenderer r{};
#else
    joj::DX11Renderer r{};
#endif 

    if (!r.init(pm->get_window()))
    {
        std::cout << "Failed to initialize renderer.\n";
        return -1;
    }

    if (r.setup_default_pipeline(pm->get_window()) != joj::ErrorCode::OK)
    {
        std::cout << "Failed to setup renderer pipeline.\n";
        return -1;
    }

    while (pm->is_running())
    {
        pm->process_events();

        if (pm->is_key_pressed(joj::KEY_ESCAPE))
            pm->close_window();

        r.clear(0.5, 0.7, 0.3, 1.0);

#if JPLATFORM_LINUX  
        pm->swap_buffers();
#else
        r.swap_buffers();
#endif 
    }
    
    pm->shutdown();
    delete pm;

    return 0;
}
