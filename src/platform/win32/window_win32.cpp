#include "platform/win32/window_win32.h"

#if JPLATFORM_WINDOWS

#include <windowsx.h>
#include <cstdio>

void (*joj::Win32Window::on_focus)() = nullptr;
void (*joj::Win32Window::lost_focus)() = nullptr;

joj::Win32Window::Win32Window()
    : m_xpos(0), m_ypos(0), m_xcenter(0), m_ycenter(0),
        m_window_config(WindowConfig {
            .handle = nullptr,
            .hdc = nullptr,
            .window_mode = WindowMode::Windowed,
            .width = static_cast<u16>(GetSystemMetrics(SM_CXSCREEN)),
            .height = static_cast<u16>(GetSystemMetrics(SM_CYSCREEN))}),
        m_window_rect({ 0, 0, 0, 0 }),
        m_client_rect({ 0, 0, 0, 0 })
{
    m_color = RGB(60, 60, 60);
    m_style = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE;
    m_icon = LoadIcon(nullptr, IDI_APPLICATION);
    m_cursor = LoadCursor(nullptr, IDC_ARROW);
}

joj::Win32Window::~Win32Window()
{
    if (m_window_config.hdc != nullptr) {
        ReleaseDC(m_window_config.handle, m_window_config.hdc);
    }

    if (m_window_config.handle != nullptr) {
        DestroyWindow(m_window_config.handle);
    }
}

void joj::Win32Window::set_mode(const WindowMode mode)
{
    if (mode == WindowMode::Windowed)
    {
        m_style = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE;
    }
    else
    {
        m_style = WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE;
    }
    
    m_window_config.window_mode = mode;
}

void joj::Win32Window::set_color(const u32 r, const u32 g, const u32 b)
{
    m_color = RGB(r, g, b);
}

joj::ErrorCode joj::Win32Window::create(const i16 width, const i16 height, const char* title, const WindowMode mode)
{
    const char* joj_wnd_class_name = "JOJ_WINDOW_CLASS";

    HINSTANCE app_id = GetModuleHandle(nullptr);

    if (!app_id)
    {
        // TODO: Use own logger and return value
        printf("Failed to get module handle.\n");
        return ErrorCode::ERR_WINDOW_CREATION;
    }

    WNDCLASSEX wnd_class{};

    if (!GetClassInfoExA(app_id, joj_wnd_class_name, &wnd_class))
    {
        wnd_class.cbSize = sizeof(WNDCLASSEX);
        wnd_class.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
        wnd_class.lpfnWndProc = WinProc;
        wnd_class.cbClsExtra = 0;
        wnd_class.cbWndExtra = 0;
        wnd_class.hInstance = app_id;
        wnd_class.hIcon = m_icon;
        wnd_class.hCursor = m_cursor;
        wnd_class.hbrBackground = CreateSolidBrush(m_color);
        wnd_class.lpszMenuName = nullptr;
        wnd_class.lpszClassName = joj_wnd_class_name;
        wnd_class.hIconSm = m_icon;

        // Register "JOJ_WINDOW_CLASS" class
        if (!RegisterClassEx(&wnd_class))
        {
            // TODO: Use own logger and return value
            printf("Failed to register window class.\n");
            return ErrorCode::ERR_WINDOW_CREATION;
        }
    }

    if (mode == WindowMode::Windowed)
    {
        m_window_config.width = width;
        m_window_config.height = height;
    }
    else if (mode == WindowMode::Fullscreen)
    {
        // Ignore width and height paremeters
        m_style = WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE;
        m_window_config.width = GetSystemMetrics(SM_CXSCREEN);
        m_window_config.height = GetSystemMetrics(SM_CYSCREEN);
    }
    else
    {
        m_style = WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE;
        m_window_config.width = width;
        m_window_config.height = height;
    }
    m_window_config.window_mode = mode;

    m_window_config.handle = CreateWindowEx(
        0,
        joj_wnd_class_name,
        title,
        m_style,
        m_xpos, m_ypos,
        m_window_config.width, m_window_config.height,
        nullptr,
        nullptr,
        app_id,
        nullptr
    );

    if (!m_window_config.handle)
    {
        // TODO: Use own logger and return value
        printf("Failed to create a window.\n");
        return ErrorCode::ERR_WINDOW_ID_CREATION;
    }

    RECT new_rect = { 0, 0, m_window_config.width, m_window_config.height };
    if (m_window_config.window_mode == WindowMode::Windowed || m_window_config.window_mode == WindowMode::Borderless)
    {
        if (!AdjustWindowRectEx(&new_rect,
            GetWindowStyle(m_window_config.handle),
            GetMenu(m_window_config.handle) != nullptr,
            GetWindowExStyle(m_window_config.handle)))
        {
            // TODO: Use own logger
            printf("Could not adjust window rect ex.\n");
        }

        m_xpos = (GetSystemMetrics(SM_CXSCREEN) / 2) - ((new_rect.right - new_rect.left) / 2);
        m_ypos = (GetSystemMetrics(SM_CYSCREEN) / 2) - ((new_rect.bottom - new_rect.top) / 2);

        if (!MoveWindow(
            m_window_config.handle,
            m_xpos,
            m_ypos,
            new_rect.right - new_rect.left,
            new_rect.bottom - new_rect.top,
            TRUE))
        {
            // TODO: Use own logger
            printf("Could not move window.\n");
        }
    }

    m_window_config.hdc = GetDC(m_window_config.handle);
    if (!m_window_config.hdc)
    {
        // TODO: Use own logger and return value
        printf("Failed to get device context.\n");
        return ErrorCode::ERR_WINDOW_CREATION;
    }

    if (!GetWindowRect(m_window_config.handle, &m_window_rect))
    {
        printf("Failed to get window size.\n");
    }

    if (!GetClientRect(m_window_config.handle, &m_client_rect))
    {
        printf("Failed to get client area size.\n");
    }

    m_running = true;

    return ErrorCode::OK;
}

void joj::Win32Window::destroy()
{
    if (m_window_config.hdc != nullptr) {
        ReleaseDC(m_window_config.handle, m_window_config.hdc);
        m_window_config.hdc = nullptr;
    }

    if (m_window_config.handle != nullptr) {
        DestroyWindow(m_window_config.handle);
        m_window_config.handle = nullptr;
    }
}

void joj::Win32Window::swap_buffers()
{
    SwapBuffers(m_window_config.hdc);
}

void joj::Win32Window::set_on_focus(void(*func)())
{
    on_focus = func;
}

void joj::Win32Window::set_lost_focus(void(*func)())
{
    lost_focus = func;
}

void joj::Win32Window::get_window_size(u16 &width, u16 &height)
{
    if (!GetWindowRect(m_window_config.handle, &m_window_rect))
    {
        printf("Failed to get window rect.");
        return;
    }

    width = m_window_rect.right - m_window_rect.left;
    height = m_window_rect.bottom - m_window_rect.top;
}

void joj::Win32Window::get_client_size(u16 &width, u16 &height)
{
    if (!GetClientRect(m_window_config.handle, &m_client_rect))
    {
        printf("Failed to get client rect.");
        return;
    }

    width = m_client_rect.right - m_client_rect.left;
    height = m_client_rect.bottom - m_client_rect.top;
}

u16 joj::Win32Window::get_xcenter()
{
    if (!GetClientRect(m_window_config.handle, &m_client_rect))
    {
        printf("Failed to get client rect.");
        return 0;
    }

    return m_client_rect.left + (m_client_rect.right / 2);
}

u16 joj::Win32Window::get_ycenter()
{
    if (!GetClientRect(m_window_config.handle, &m_client_rect))
    {
        printf("Failed to get client rect.");
        return 0;
    }

    return m_client_rect.top + (m_client_rect.bottom / 2);
}

LRESULT CALLBACK joj::Win32Window::WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    // FIXME: Window always close when creating OpenGL context
    // case WM_DESTROY:
    case WM_QUIT:
    case WM_CLOSE:
        printf("Closing window.\n");
        PostQuitMessage(0);
        return 0;

    case WM_KILLFOCUS:
        if (lost_focus)
            lost_focus();
        return 0;

    case WM_SETFOCUS:
        if (on_focus)
            on_focus();
        return 0;

    default:
        break;
    }
    
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

#endif // JPLATFORM_WINDOWS
