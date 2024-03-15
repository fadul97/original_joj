#include "engine.h"

#include <iostream>

joj::Engine::Engine()
{
    platform_manager = new Win32PlatformManager();
    renderer = new DX11Renderer();
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

joj::ErrorCode joj::Engine::run()
{
    if (init() != ErrorCode::OK)
    {
        std::cout << "Failed to initialize engine.\n";
        return ErrorCode::ERR_ENGINE_INIT;
    }

    while (platform_manager->is_running())
    {
        platform_manager->process_events();

        if (platform_manager->is_key_pressed(joj::KEY_ESCAPE))
            platform_manager->close_window();

        renderer->clear(0, 0, 1, 1);

        renderer->swap_buffers();
    }

    shutdown();

    return ErrorCode::OK;
}

void joj::Engine::shutdown()
{
    renderer->shutdown();
    platform_manager->shutdown();
}
