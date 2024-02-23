#include "platform/win32/platform_manager_win32.h"

#if JPLATFORM_WINDOWS

joj::Win32PlatformManager::Win32PlatformManager()
{
    window = nullptr;
    input = nullptr;
    context = nullptr;
    msg = { 0 };
}

joj::Win32PlatformManager::~Win32PlatformManager()
{
}

b8 joj::Win32PlatformManager::init(i16 width, i16 height, std::string title)
{
    window = std::make_unique<Win32Window>(width, height, title);
    
    window->set_mode(joj::WindowMode::WINDOWED);
    window->set_color(60, 60, 60);
    window->create();
    
    return true;
}

b8 joj:: Win32PlatformManager::create_window()
{
    if (!window->create())
    {
        // TODO: use own logger and return value, cleanup?
        printf("Failed to initialize Win32Window.\n");
        return false;
    }
    
    // ATTENTION: input must be initialized after window creation
    input = std::make_unique<Win32Input>();

    context = std::make_unique<Win32GLContext>();
    
    if (!context->create(window))
    {
        // TODO: use own logger and return value, cleanup?
        printf("Failed to initialize GLContext.\n");
        return false;
    }

    // FIXME: Check make_current method
    context->make_current(window);

    return true;
}

b8 joj::Win32PlatformManager::create_simple_window(i16 width, i16 height, std::string title)
{
    // TODO: use own logger and return value
    printf("TODO()!\n");
    return true;
}

void joj::Win32PlatformManager::process_events()
{
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void joj::Win32PlatformManager::swap_buffers()
{
    SwapBuffers(window->get_device_context());
}

void joj::Win32PlatformManager::shutdown()
{
    window->shutdown();
}

void joj::Win32PlatformManager::set_window_icon(i32 count, IconImage& image)
{
    
}

#endif // JPLATFORM_WINDOWS
