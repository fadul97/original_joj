#include "engine.h"

#include <stdio.h>
#include <iostream>
#include <sstream>
#include "app.h"

joj::JPlatformManager* joj::Engine::platform_manager = nullptr;
joj::JRenderer* joj::Engine::renderer = nullptr;
joj::JGLRenderer* joj::Engine::gl_renderer = nullptr;
std::vector<joj::Error> joj::Engine::errors = std::vector<joj::Error>();

b8 joj::Engine::m_paused = false;

joj::Engine::Engine()
{
    platform_manager = new JPlatformManager();
    renderer = nullptr;
    gl_renderer = nullptr;
    m_frametime = 0.0f;
}

joj::Engine::~Engine()
{
    if (renderer)
        delete renderer;
    
    if (gl_renderer)
        delete gl_renderer;
    
    if (platform_manager)
        delete platform_manager;
}

joj::ErrorCode joj::Engine::init(RendererBackend renderer_backend)
{
    platform_manager->init(800, 600);
    if (!platform_manager->init(800, 600))
    {
        // TODO: Use own logger
        std::cout << "Failed to initialize Platform Manager.\n";
        return ErrorCode::ERR_PLATFORM_MANAGER_INIT;
    }

    if (!platform_manager->create_window())
    {
        // TODO: Use own logger
        std::cout << "Failed to create window.\n";
        return ErrorCode::ERR_PLATFORM_MANAGER_CREATION;
    }

    switch (renderer_backend)
    {
    case RendererBackend::DX11:
        renderer = new JRenderer();
        if (!renderer->init(platform_manager->get_window()))
        {
            std::cout << "Failed to initialize renderer.\n";
            return ErrorCode::ERR_PLATFORM_MANAGER_CREATION;
        }

        if (renderer->setup_default_pipeline(platform_manager->get_window()) != joj::ErrorCode::OK)
        {
            std::cout << "Failed to setup renderer pipeline.\n";
            return ErrorCode::ERR_RENDERER_PIPELINE_ERROR;
        }

        break;

    case RendererBackend::GL:
        gl_renderer = new JGLRenderer();
        if (!gl_renderer->init(platform_manager->get_window()))
        {
            std::cout << "Failed to initialize renderer.\n";
            return ErrorCode::ERR_PLATFORM_MANAGER_CREATION;
        }

        if (gl_renderer->setup_default_pipeline(platform_manager->get_window()) != joj::ErrorCode::OK)
        {
            std::cout << "Failed to setup renderer pipeline.\n";
            return ErrorCode::ERR_RENDERER_PIPELINE_ERROR;
        }

        break;

    }

    return ErrorCode::OK;
}

joj::ErrorCode joj::Engine::run(App* app, RendererBackend renderer_backend)
{
    if (init(renderer_backend) != ErrorCode::OK)
    {
        std::cout << "Failed to initialize engine.\n";
        return ErrorCode::ERR_ENGINE_INIT;
    }

    // Adjust sleep resolution to 1 millisecond
    platform_manager->begin_period();

#if JPLATFORM_WINDOWS
    // Game pauses/resumes when losing/gaining focus
    platform_manager->set_lost_focus(pause);
    platform_manager->set_on_focus(resume);
#endif

    // Start time counter
    platform_manager->start_timer();

    app->init();

    f32 dt = 1.0f;

    while (platform_manager->is_running())
    {
        if (!platform_manager->process_events())
        {
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
                app->update(m_frametime);

                // Game draw
                app->draw();
            }
            else
            {
                // Game paused
                app->on_pause();
            }
        }
    }

    app->shutdown();

    if (!errors.empty())
        ouput_log();

    shutdown();

    return ErrorCode::OK;
}

void joj::Engine::shutdown()
{
    renderer->shutdown();
    platform_manager->shutdown();
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

        SetWindowText(platform_manager->get_window()->get_id(), text.str().c_str());

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
