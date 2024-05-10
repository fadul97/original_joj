#include <iostream>
#include "joj/platform/win32/window_win32.h"
#include "joj/platform/context/opengl/win32/context_gl.h"

b8 process_events();

int main()
{
    joj::Win32Window window{};
    if (window.create(800, 600, "Refactored window", joj::WindowMode::Windowed) < joj::ErrorCode::OK) {
        return -1;
    }

    u16 width = 0;
    u16 height = 0;
    window.get_window_size(width, height);
    printf("Window size: %dx%d\n", width, height);

    window.get_client_size(width, height);
    printf("Client size: %dx%d\n", width, height);

    joj::Win32GLContext context{};
    if (context.create(window.get_window_config()) < joj::ErrorCode::OK) {
        window.destroy();
        return -2;
    }
    //
    // if (context.make_current(window.get_window_config()) < joj::ErrorCode::OK) {
    //     window.destroy();
    //     return -3;
    // }

    while (window.is_running()) {
        if (!process_events()) {
            window.stop_running();
        }
    }

    window.destroy();

    printf("Hello, Refactoring!\n");
    return 0;
}

b8 process_events() {
    MSG msg{};

    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (msg.message == WM_QUIT || msg.message == WM_DESTROY)
            return false;
    }

    return true;
}