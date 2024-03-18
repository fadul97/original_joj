#include "engine.h"

#include <iostream>
#include <sstream>

joj::Win32PlatformManager* joj::Engine::platform_manager = nullptr;
joj::DX11Renderer* joj::Engine::renderer = nullptr;
b8 joj::Engine::m_paused = false;

joj::Engine::Engine()
{
    platform_manager = new Win32PlatformManager();
    renderer = new DX11Renderer();
    m_frametime = 0.0f;
}

joj::Engine::~Engine()
{
    delete renderer;
    delete platform_manager;
}

joj::ErrorCode joj::Engine::init()
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

    return ErrorCode::OK;
}

joj::ErrorCode joj::Engine::run(App* app)
{
    if (init() != ErrorCode::OK)
    {
        std::cout << "Failed to initialize engine.\n";
        return ErrorCode::ERR_ENGINE_INIT;
    }

    // Adjust sleep resolution to 1 millisecond
    platform_manager->begin_period();

    // Game pauses/resumes when losing/gaining focus
    platform_manager->set_lost_focus(pause);
    platform_manager->set_on_focus(resume);

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
            if (platform_manager->is_key_pressed(joj::KEY_P))
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