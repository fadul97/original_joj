#include "platform/win32/platform_manager_win32.h"

#if JPLATFORM_WINDOWS

joj::Win32PlatformManager::Win32PlatformManager()
{
    m_context = nullptr;
    m_timer = nullptr;
}

joj::Win32PlatformManager::~Win32PlatformManager()
{
}

joj::ErrorCode joj::Win32PlatformManager::create_window(i16 width, i16 height, const char* title, WindowMode mode)
{
    m_window = std::make_unique<Win32Window>(width, height, title);
    m_window->set_mode(mode);
    m_window->set_color(60, 60, 60);

    if (!m_window->create())
    {
        // TODO: Use own logger
        printf("Failed to create window.\n");
        return ErrorCode::ERR_WINDOW_CREATION;
    }

    return ErrorCode::OK;
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

joj::ErrorCode joj::Win32PlatformManager::create_input()
{
    m_input = std::make_unique<Win32Input>();

    if (m_input == nullptr)
    {
        // TODO: Use own logger
        printf("Failed to create input.\n");
        return ErrorCode::ERR_INPUT_CREATION;
    }

    return ErrorCode::OK;
}

joj::ErrorCode joj::Win32PlatformManager::create_timer()
{
    m_timer = std::make_unique<Win32Timer>();

    if (m_timer == nullptr)
    {
        // TODO: Use own logger
        printf("Failed to create timer.\n");
        return ErrorCode::ERR_TIMER_CREATION;
    }

    return ErrorCode::OK;
}

joj::ErrorCode joj::Win32PlatformManager::make_gl_context_current(std::unique_ptr<Win32Window>& window)
{
    if (m_context == nullptr)
    {
        m_context = std::make_unique<Win32GLContext>();

        if (!m_context->create(window))
        {
            // TODO: use own logger, cleanup?
            printf("Failed to create GLContext.\n");
            return ErrorCode::ERR_CONTEXT_CREATION;
        }
    }

    m_context->make_current(window);
    
    return ErrorCode::OK;
}

joj::ErrorCode joj::Win32PlatformManager::create_context(std::unique_ptr<Win32Window>& window, BackendRenderer backend_renderer)
{
    switch (backend_renderer)
    {
    case BackendRenderer::GL:
    {
        m_context = std::make_unique<Win32GLContext>();

        if (!m_context->create(window))
        {
            // TODO: use own logger, cleanup?
            printf("Failed to create GLContext.\n");
            return ErrorCode::ERR_CONTEXT_CREATION;
        }

        m_context->make_current(window);

        return ErrorCode::OK;
    }
    
    default:
        return ErrorCode::FAILED;
    }

    return ErrorCode::FAILED;
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

b8 joj::Win32PlatformManager::process_events()
{
    MSG msg = { 0 };

    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
 
        if (msg.message == WM_QUIT || msg.message == WM_DESTROY)
            return false;
    }

    return true;
}

void joj::Win32PlatformManager::change_window_procedure(std::unique_ptr<Win32Window>& window, void* func)
{
    SetWindowLongPtr(window->get_id(), GWLP_WNDPROC, (LONG_PTR)func);
}

void joj::Win32PlatformManager::shutdown()
{
    m_window->close();
}

void joj::Win32PlatformManager::set_window_title(std::unique_ptr<Win32Window>& window, const char* title)
{
    SetWindowText(window->get_id(), title);
}

#endif // JPLATFORM_WINDOWS
