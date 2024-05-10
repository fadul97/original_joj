#include <iostream>
#include <sstream>

#include "joj/platform/win32/window_win32.h"
#include "joj/platform/win32/input_win32.h"
#include "joj/platform/win32/timer_win32.h"
#include "joj/platform/context/opengl/win32/context_gl.h"

f32 frametime;

joj::Win32Window window{};
joj::Win32Timer timer{};

b8 process_events();
void update_frametime();

int main()
{
    if (window.create(800, 600, "Refactored window", joj::WindowMode::Windowed) < joj::ErrorCode::OK) {
        return -1;
    }

    auto input = joj::Win32Input();

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

    if (context.make_current(window.get_window_config()) < joj::ErrorCode::OK) {
        window.destroy();
        return -3;
    }


    timer.begin_period();
    timer.start();

    if (window.get_window_config().handle == nullptr) {
        printf("NULL handle.\n");
    } else {
        printf("NOT null handle.\n");
    }

    while (window.is_running()) {
        if (!process_events()) {
            window.stop_running();
        }

        update_frametime();

        if (joj::Win32Input::is_key_pressed('A')) {
            printf("A pressed.\n");
        }

        if (joj::Win32Input::is_key_pressed(joj::KEY_ESCAPE)) {
            printf("Escape key pressed.\n");
        }

        window.swap_buffers();

    }

    timer.end_period();
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

void update_frametime() {
    static f32 total_time = 0.0f;	// Total time elapsed
    static u32  frame_count = 0;	// Elapsed frame counter

    // Current frame time
    frametime = timer.reset();

    // Accumulated frametime
    total_time += frametime;

    // Increment frame counter
    frame_count++;

    // Updates FPS indicator in the window every 1000ms (1 second)
    if (total_time >= 1.0f)
    {
        std::stringstream text;		// Text flow for messages
        text << std::fixed;			// Always show the fractional part
        text.precision(3);			// three numbers after comma

        text << "Renderer Backend: " << "OpenGL" << "    "
            << "FPS: " << frame_count << "    "
            << "Frametime: " << frametime * 1000 << " (ms)";

        window.set_title(text.str().c_str());

        frame_count = 0;
        total_time -= 1.0f;
    }
}