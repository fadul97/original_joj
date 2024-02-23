// FIXME: Main file doesn't need to define JOJ_ENGINE_IMPLEMENTATION
#define JOJ_ENGINE_IMPLEMENTATION
#include "joj/defines.h"
#include <iostream>

#if JPLATFORM_LINUX  
#include "joj/platform/x11/platform_manager_x11.h"
#include "renderer/opengl/renderer_gl.h"
#elif JPLATFORM_WINDOWS
#include <memory>
#include "joj/platform/win32/window_win32.h"
#include "joj/platform/win32/input_win32.h"
#include "joj/platform/win32/timer_win32.h"
#include "joj/platform/win32/platform_manager_win32.h"
#include "joj/graphics/opengl/win32/context_gl.h"
#define JOJ_GL_DEFINE_EXTERN
#include "joj/graphics/opengl/joj_gl.h"
#include "joj/joj.h"
#endif

int main()
{
#if JPLATFORM_LINUX  
    std::cout << "Hello, Linux!\n";
    joj::X11PlatformManager pm{};
    pm.init();
    pm.create_window();

    joj::GLRenderer renderer{};
    renderer.init(pm.get_window());

    while (pm.is_running())
    {
        pm.process_events();

        if (pm.is_key_down(KEY_ESCAPE))
            pm.close_window();

        renderer.clear(0.5f, 1.0f, 0.5f, 1.0f);
        pm.swap_buffers();
    }

    pm.shutdown();
#elif JPLATFORM_WINDOWS
    joj::Win32PlatformManager pm{};
    pm.init();
    pm.create_window();

    // joj::GLRenderer renderer{};
    // renderer.init(pm.get_window());

    while (pm.is_running())
    {
        pm.process_events();

        if (pm.is_key_down(joj::KEY_ESCAPE))
            pm.close_window();

        // renderer.clear(0.5f, 1.0f, 0.5f, 1.0f);
        pm.swap_buffers();
    }

    pm.shutdown();
#endif // JPLATFORM_WINDOWS

    return 0;
}
