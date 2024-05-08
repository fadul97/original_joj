#include <cstdio>
#include "joj/platform/win32/window_win32.h"
#include "joj/platform/win32/input_win32.h"

b8 process_events();

int main()
{
    joj::Win32Window window{};
    if (window.create(400, 400, "Refactored window", joj::WindowMode::Windowed) < joj::ErrorCode::OK) {
        return -1;
    }

    joj::Win32Input input{};

    u16 width = 0;
    u16 height = 0;
    window.get_window_size(width, height);
    printf("Window size: %dx%d\n", width, height);

    window.get_client_size(width, height);
    printf("Client size: %dx%d\n", width, height);

    while (window.is_running()) {
        if (!process_events()) {
            window.stop_running();
        }

        if (joj::Win32Input::is_key_pressed('A')) {
            printf("A pressed.\n");
        }

        if (joj::Win32Input::is_key_pressed(joj::KEY_ESCAPE)) {
            printf("Escape key pressed.\n");
        }

        window.swap_buffers();

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
