#include "platform/win32/platform_manager_win32.h"

#if JPLATFORM_WINDOWS

joj::Win32PlatformManager::Win32PlatformManager()
{
}

joj::Win32PlatformManager::~Win32PlatformManager()
{
}

std::unique_ptr<joj::Win32Window> joj::Win32PlatformManager::create_window(i16 width, i16 height, const char* title, WindowMode mode)
{
    auto window = std::make_unique<Win32Window>(width, height, title);
    window->set_mode(mode);
    window->set_color(60, 60, 60);

    if (!window->create())
    {
        // TODO: Use own logger
        printf("Failed to create window.\n");
        return nullptr;
    }

    return window;
}

std::unique_ptr<joj::Win32Window> joj::Win32PlatformManager::create_simple_window(i16 width, i16 height, const char* title)
{
    auto window = std::make_unique<Win32Window>(width, height, title);

    if (!window->create())
    {
        // TODO: Use own logger
        printf("Failed to create window.\n");
        return nullptr;
    }

    return window;
}

std::unique_ptr<joj::Win32Input> joj::Win32PlatformManager::create_input()
{
    auto input = std::make_unique<Win32Input>();

    if (input == nullptr)
    {
        // TODO: Use own logger
        printf("Failed to create input.\n");
        return nullptr;
    }

    return input;
}

std::unique_ptr<joj::Timer> joj::Win32PlatformManager::create_timer()
{
    auto timer = std::make_unique<Win32Timer>();

    if (timer == nullptr)
    {
        // TODO: Use own logger
        printf("Failed to create timer.\n");
        return nullptr;
    }

    return timer;
}

std::unique_ptr<joj::GraphicsContext<joj::Win32Window>> joj::Win32PlatformManager::create_context(std::unique_ptr<Win32Window>& window, BackendRenderer backend_renderer)
{
    switch (backend_renderer)
    {
    case BackendRenderer::GL:
    {
        auto context = std::make_unique<Win32GLContext>();

        if (!context->create(window))
        {
            // TODO: use own logger, cleanup?
            printf("Failed to create GLContext.\n");
            return nullptr;
        }

        context->make_current(window);

        return context;
    }

    case BackendRenderer::DX11:
        auto context = std::make_unique<DX11Context>();
        
        if (!context->create(window))
        {
            // TODO: use own logger, cleanup?
            printf("Failed to create DX11Context.\n");
            return nullptr;
        }

        return context;
    }

    return nullptr;
}

joj::ErrorCode joj::Win32PlatformManager::create_window_and_context(std::unique_ptr<Win32Window>& window, BackendRenderer backend_renderer)
{
    // TODO: use own logger
    printf("TODO()!\n");
    return ErrorCode::ERR_PLATFORM_MANAGER_CREATION;
}

void joj::Win32PlatformManager::set_window_icon(std::unique_ptr<Win32Window>& window, i32 count, IconImage& image)
{
    // TODO: use own logger
    printf("TODO()!\n");
}

#endif // JPLATFORM_WINDOWS
