#include <iostream>

#include "joj/joj.h"
#include "joj/defines.h"
#include "joj/platform/window.h"
#include "joj/platform/x11/window_x11.h"
#include "joj/platform/platform_manager.h"
#include "joj/platform/x11/platform_manager_x11.h"
#include "joj/platform/input.h"
#include "joj/platform/timer.h"
#include "joj/platform/x11/timer_x11.h"

int main()
{
#if JPLATFORM_LINUX
    std::cout << "LINUX!\n";
#elif JPLATFORM_WINDOWS
    std::cout << "WINDOWS!\n";
#endif

    auto pm = new joj::X11PlatformManager();
    pm->create_simple_window(800, 600);
    while (pm->is_running())
    {
        pm->process_events();

        if (pm->is_key_pressed(KEY_SPACE))
            std::cout << "KEY_SPACE!\n";

        if (pm->is_key_down(KEY_ENTER) && pm->is_key_down(KEY_A))
            std::cout << "KEY_ENTER!\n";

        if (pm->is_key_pressed(KEY_ESCAPE))
            pm->close_window();
    }

    pm->shutdown();
    delete pm;
    
    std::cout << "Hello, Joj!\n";
    joj_print();

    u8 a = 0;
    std::cout << "a = " << a << "\n";

    u32 b = 10;
    std::cout << "b = " << b << "\n";

    i32 c = -10;
    std::cout << "c = " << c << "\n";
    return 0;
}
