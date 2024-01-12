#include <iostream>

#include "joj/joj.h"
#include "joj/defines.h"
#include "joj/platform/window.h"
#include "joj/platform/x11/window_x11.h"

#include <unistd.h>

int main()
{
#if JPLATFORM_LINUX
    std::cout << "LINUX!\n";
#elif JPLATFORM_WINDOWS
    std::cout << "WINDOWS!\n";
#endif

    auto window = new joj::X11Window();
    window->create_simple_window();
    window->show();
    while (window->is_running())
    {
        sleep(3);
        window->close();
    }

    window->shutdown();
    delete window;
    
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
