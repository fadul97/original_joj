#include "engine.h"

#include <stdio.h>
#include <iostream>
#include <sstream>
#include "app.h"

joj::Engine* joj::Engine::engine = nullptr;
std::unique_ptr<joj::JPlatformManager> joj::Engine::platform_manager = nullptr;
std::unique_ptr<joj::JRenderer> joj::Engine::dx11_renderer = nullptr;
std::unique_ptr<joj::JGLRenderer> joj::Engine::gl_renderer = nullptr;

std::vector<joj::Error> joj::Engine::errors = std::vector<joj::Error>();

b8 joj::Engine::m_paused = false;
b8 joj::Engine::m_running = false;

joj::App* joj::Engine::m_app = nullptr;

joj::Engine::Engine()
{
    platform_manager = std::make_unique<JPlatformManager>();
    m_frametime = 0.0f;
}

joj::Engine::~Engine()
{
}

joj::ErrorCode joj::Engine::init(BackendRenderer backend_renderer)
{
    JOUTPUTFAILED(platform_manager->create_window(800, 600, "My JWindow", WindowMode::WINDOWED));

    // ATTENTION: input must be initialized after window creation
    JOUTPUTFAILED(platform_manager->create_input());

    switch (backend_renderer)
    {
    case BackendRenderer::GL:
    {
        platform_manager->make_gl_context_current(platform_manager->get_window());
        gl_renderer = std::make_unique<JGLRenderer>();
        gl_renderer->init(platform_manager->get_window());
        gl_renderer->setup_default_pipeline(platform_manager->get_window());
        break;
    }

    case BackendRenderer::DX11:
        dx11_renderer = std::make_unique<JRenderer>();
        dx11_renderer->init(platform_manager->get_window());
        dx11_renderer->setup_default_pipeline(platform_manager->get_window());
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

    platform_manager->change_window_procedure(platform_manager->get_window(), EngineProc);

    // Adjust sleep resolution to 1 millisecond
    JOUTPUTFAILED(platform_manager->create_timer());
    platform_manager->begin_period();

#if JPLATFORM_WINDOWS
    // Game pauses/resumes when losing/gaining focus
    platform_manager->set_lost_focus(pause);
    platform_manager->set_on_focus(resume);
#endif

    // Start time counter
    platform_manager->start_timer();

    m_app = app;
    m_app->init();

    m_running = true;

    while (m_running)
    {
        if (!platform_manager->process_events())
            m_running = false;

        // -----------------------------------------------
        // Pause/Resume Game
        // -----------------------------------------------

        // P key pauses engine
        if (platform_manager->is_key_pressed('P'))
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

    m_app->shutdown();

    if (!errors.empty())
        ouput_log();

    shutdown();

    delete engine;

    return ErrorCode::OK;
}

void joj::Engine::shutdown()
{
    platform_manager->shutdown();

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
    m_frametime = platform_manager->reset_timer();

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

        text << platform_manager->get_window()->get_title().c_str() << "    "
            << "Renderer Backend: " << "DirectX 11" << "    "
            << "FPS: " << frame_count << "    "
            << "Frametime: " << m_frametime * 1000 << " (ms)";

        platform_manager->set_window_title(platform_manager->get_window(), text.str().c_str());

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

#if JPLATFORM_WINDOWS
LRESULT CALLBACK joj::Engine::EngineProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // Window must be repainted
    if (msg == WM_PAINT)
        m_app->display();

    return CallWindowProc(joj::Win32Input::InputProc, hWnd, msg, wParam, lParam);
}
#endif // JPLATFORM_WINDOWS