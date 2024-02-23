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
    auto w = std::make_unique<joj::Win32Window>();
    w->set_mode(joj::WindowMode::WINDOWED);
    w->set_color(60, 60, 60);
    w->create();
    joj::Win32Input i{};

    joj::Win32GLContext c{};
    if (!c.create(w))
        std::cout << "Failed to create OpenGL context.\n";
    c.make_current(w);
    
    b8 r = true;
    MSG msg = { 0 };
    while (r && msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        if (i.is_key_pressed(joj::KEY_ESCAPE))
            r = false;
        
        if (i.is_key_pressed(joj::KEY_LEFT))
            std::cout << "LEFT ARROW\n";
        
        if (i.is_key_pressed(joj::KEY_RIGHT))
            std::cout << "RIGHT ARROW\n";
        
        if (i.is_key_pressed(joj::KEY_UP))
            std::cout << "UP ARROW\n";
        
        if (i.is_key_pressed(joj::KEY_DOWN))
            std::cout << "DOWN ARROW\n";

        glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        SwapBuffers(w->get_device_context());
    }
    
    w->shutdown();
    joj::joj_print();
#endif // JPLATFORM_WINDOWS

    return 0;
}
