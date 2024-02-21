#include "joj/defines.h"
#include <iostream>

#if JPLATFORM_LINUX  
#include "joj/platform/x11/platform_manager_x11.h"
#include "renderer/opengl/renderer_gl.h"
#endif // JPLATFORM_LINUX 

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
    std::cout << "Hello, Windows!\n";
#endif // JPLATFORM_WINDOWS

    return 0;
}
