#include "engine.h"

#include <stdio.h>
#include <iostream>
#include <sstream>
#include "app.h"

joj::Engine* joj::Engine::engine = nullptr;
joj::JPlatformManager* joj::Engine::platform_manager = nullptr;
std::unique_ptr<joj::JRenderer> joj::Engine::dx11_renderer = nullptr;
std::unique_ptr<joj::JGLRenderer> joj::Engine::gl_renderer = nullptr;
std::unique_ptr<joj::JWindow> joj::Engine::window = nullptr;

// ATTENTION: input must be initialized after window creation
std::unique_ptr<joj::JInput> joj::Engine::input = nullptr;

std::unique_ptr<joj::JTimer> joj::Engine::timer = nullptr;
std::vector<joj::Error> joj::Engine::errors = std::vector<joj::Error>();

b8 joj::Engine::m_paused = false;

joj::App* joj::Engine::m_app = nullptr;

joj::Engine::Engine()
{
    platform_manager = new JPlatformManager();
    m_frametime = 0.0f;
}

joj::Engine::~Engine()
{
    if (platform_manager)
        delete platform_manager;
}

joj::ErrorCode joj::Engine::init(BackendRenderer backend_renderer)
{
    window = platform_manager->create_window(800, 600, "My JWindow", WindowMode::WINDOWED);
    if (window == nullptr)
        return ErrorCode::ERR_WINDOW_CREATION;

    // ATTENTION: input must be initialized after window creation
    input = platform_manager->create_input();

    switch (backend_renderer)
    {
    case BackendRenderer::GL:
    {
        auto context = platform_manager->create_context(window, backend_renderer);
        gl_renderer = std::make_unique<JGLRenderer>();
        gl_renderer->init(window);
        gl_renderer->setup_default_pipeline(window);
        break;
    }

    case BackendRenderer::DX11:
        dx11_renderer = std::make_unique<JRenderer>();
        dx11_renderer->init(window);
        dx11_renderer->setup_default_pipeline(window);
        break;
    }

    return ErrorCode::OK;
}

joj::ErrorCode joj::Engine::run(App* app, BackendRenderer backend_renderer)
{
    if (init(backend_renderer) != ErrorCode::OK)
    {
        std::cout << "Failed to initialize engine.\n";
        return ErrorCode::ERR_ENGINE_INIT;
    }

    SetWindowLongPtr(GetActiveWindow(), GWLP_WNDPROC, (LONG_PTR)EngineProc);

    // Adjust sleep resolution to 1 millisecond
    timer = platform_manager->create_timer();
    timer->time_begin_period();

#if JPLATFORM_WINDOWS
    // Game pauses/resumes when losing/gaining focus
    window->set_lost_focus(pause);
    window->set_on_focus(resume);
#endif

    // Start time counter
    timer->start();

    m_app = app;
    m_app->init();

    MSG msg = { 0 };

    while (window->is_running() && msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // -----------------------------------------------
            // Pause/Resume Game
            // -----------------------------------------------

            // P key pauses engine
            if (input->is_key_pressed('P'))
            {
                if (m_paused)
                    resume();
                else
                    pause();
            }

            if (!m_paused)
            {
                // Calculate frametime
                m_frametime = get_frametime();

                // Update game
                m_app->update(m_frametime);

                // Game draw
                m_app->draw();
            }
            else
            {
                // Game paused
                m_app->on_pause();
            }
        }
    }

    // Close game
    printf("Message: %d\n", i32(msg.wParam));

    m_app->shutdown();

    if (!errors.empty())
        ouput_log();

    shutdown();

    delete engine;

    return ErrorCode::OK;
}

void joj::Engine::shutdown()
{
    window->close();

    if (dx11_renderer)
        dx11_renderer->shutdown();

    if (gl_renderer)
        gl_renderer->shutdown();
}

f32 joj::Engine::get_frametime()
{

#ifdef _DEBUG
    static f32 total_time = 0.0f;	// Total time elapsed
    static u32  frame_count = 0;	// Elapsed frame counter
#endif

    // Current frame time
    m_frametime = timer->reset();

#ifdef _DEBUG
    // Accumulated frametime
    total_time += m_frametime;

    // Increment frame counter
    frame_count++;

    // Updates FPS indicator in the window every 1000ms (1 second)
    if (total_time >= 1.0f)
    {
        std::stringstream text;		// Text flow for messages
        text << std::fixed;			// Always show the fractional part
        text.precision(3);			// three numbers after comma

        text << window->get_title().c_str() << "    "
            << "Renderer Backend: " << "DirectX 11" << "    "
            << "FPS: " << frame_count << "    "
            << "Frametime: " << m_frametime * 1000 << " (ms)";

        SetWindowText(window->get_id(), text.str().c_str());

        frame_count = 0;
        total_time -= 1.0f;
    }
#endif

    return m_frametime;
}

void joj::Engine::ouput_log() const
{
    for (Error e : errors)
        e.what();
}

LRESULT CALLBACK joj::Engine::EngineProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // Window must be repainted
    if (msg == WM_PAINT)
        m_app->display();

    return CallWindowProc(joj::Win32Input::InputProc, hWnd, msg, wParam, lParam);
}