#include "platform/win32/platform_manager_win32.h"

#if JPLATFORM_WINDOWS

joj::Win32PlatformManager::Win32PlatformManager()
{
    m_window = std::make_unique<Win32Window>();
    m_timer = std::make_unique<Win32Timer>();

    // ATTENTION: input must be initialized after window creation
    m_input = nullptr;
    
    m_dx11_context = nullptr;
    m_gl_context = nullptr;
    
    msg = { 0 };
}

joj::Win32PlatformManager::~Win32PlatformManager()
{
}

joj::ErrorCode joj::Win32PlatformManager::init(i16 width, i16 height, std::string title, WindowMode mode)
{
    m_window = std::make_unique<Win32Window>(width, height, title);
    
    m_window->set_mode(mode);
    m_window->set_color(60, 60, 60);

    return ErrorCode::OK;
}

joj::ErrorCode joj:: Win32PlatformManager::create_window()
{
    if (!m_window->create())
    {
        // TODO: use own logger and return value, cleanup?
        printf("Failed to initialize Win32Window.\n");
        return ErrorCode::ERR_WINDOW_CREATION;
    }
    
    // ATTENTION: input must be initialized after window creation
    m_input = std::make_unique<Win32Input>();

    return ErrorCode::OK;
}

joj::ErrorCode joj::Win32PlatformManager::create_simple_window(i16 width, i16 height, std::string title, WindowMode mode)
{
    // TODO: use own logger
    printf("TODO()!\n");
    return ErrorCode::FAILED;
}

joj::ErrorCode joj::Win32PlatformManager::create_context(BackendRenderer backend_renderer)
{
    switch (backend_renderer)
    {
    case BackendRenderer::GL:
        m_gl_context = std::make_unique<Win32GLContext>();
    
        if (!m_gl_context->create(m_window))
        {
            // TODO: use own logger, cleanup?
            printf("Failed to initialize GLContext.\n");
            return ErrorCode::ERR_CONTEXT_CREATION;
        }

        // FIXME: Check make_current method
        m_gl_context->make_current(m_window);

        return ErrorCode::OK;

    case BackendRenderer::DX11:
        m_dx11_context = std::make_unique<DX11Context>();
        
        if (!m_dx11_context->create(m_window))
        {
            // TODO: use own logger
            printf("Failed to initialize DX11Context.\n");
            return ErrorCode::ERR_CONTEXT_CREATION;
        }

        // FIXME: Check make_current method
        m_dx11_context->make_current(m_window);
        
        return ErrorCode::OK;
    }

    return ErrorCode::FAILED;
}

joj::ErrorCode joj::Win32PlatformManager::create_window_and_context(BackendRenderer backend_renderer)
{
    // TODO: use own logger
    printf("TODO()!\n");
    return ErrorCode::ERR_PLATFORM_MANAGER_CREATION;
}

b8 joj::Win32PlatformManager::process_events()
{
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        return true;
    }

    return false;
}

void joj::Win32PlatformManager::swap_buffers()
{
    SwapBuffers(m_window->get_device_context());
}

void joj::Win32PlatformManager::shutdown()
{
    if (m_gl_context)
        m_gl_context->destroy();

    if (m_dx11_context)
        m_dx11_context->destroy();

    m_window->shutdown();
}

void joj::Win32PlatformManager::set_window_icon(i32 count, IconImage& image)
{
    // TODO: use own logger
    printf("TODO()!\n");
}

#endif // JPLATFORM_WINDOWS
