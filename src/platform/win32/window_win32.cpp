#include "platform/win32/window_win32.h"

#include <stdio.h>

void (*joj::Win32Window::on_focus)() = nullptr;
void (*joj::Win32Window::lost_focus)() = nullptr;

joj::Win32Window::Win32Window(i16 width, i16 height, std::string title)
    : joj::Window<HWND>(width, height, title)
{
    m_id = 0;
    m_hdc = { 0 };
    m_rect = { 0, 0, 0, 0 };
    m_icon = LoadIcon(NULL, IDI_APPLICATION);
    m_cursor = LoadCursor(NULL, IDC_ARROW);
    m_color = RGB(0, 0, 0);
    m_style = WS_POPUP | WS_VISIBLE;
    m_mode = WindowMode::FULLSCREEN;
    m_xpos = 0;
    m_ypos = 0;
    m_xcenter = width / 2;
    m_ycenter = height / 2;
}

joj::Win32Window::~Win32Window()
{
}

void joj::Win32Window::hide_cursor(b8 hide)
{
    ShowCursor(!hide);
}

void joj::Win32Window::set_mode(WindowMode mode)
{
    if (mode == WindowMode::WINDOWED)
    {
        m_style = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE;
    }
    else
    {
        m_style = WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE;
    }
    
    m_mode = mode;
}

void joj::Win32Window::set_color(u32 r, u32 g, u32 b)
{
    m_color = RGB(r, g, b);
}

b8 joj::Win32Window::init()
{
    // TODO: use own logger and return value
    printf("TODO()!\n");
    return false;
}

b8 joj::Win32Window::create()
{
    const char* joj_wnd_class_name = "JOJ_WINDOW_CLASS";

    HINSTANCE app_id = GetModuleHandle(NULL);

    if (!app_id)
    {
        // TODO: Use own logger and return value
        printf("Failed to get module handle.\n");
        return false;
    }

    WNDCLASSEX wnd_class = { };

    if (!GetClassInfoExA(app_id, joj_wnd_class_name, &wnd_class))
    {
        wnd_class.cbSize = sizeof(WNDCLASSEX);
        wnd_class.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
        wnd_class.lpfnWndProc = joj::Win32Window::WinProc;
        wnd_class.cbClsExtra = 0;
        wnd_class.cbWndExtra = 0;
        wnd_class.hInstance = app_id;
        wnd_class.hIcon = m_icon;
        wnd_class.hCursor = m_cursor;
        wnd_class.hbrBackground = (HBRUSH)CreateSolidBrush(m_color);
        wnd_class.lpszMenuName = NULL;
        wnd_class.lpszClassName = joj_wnd_class_name;
        wnd_class.hIconSm = m_icon;

        // Register "JOJ_WINDOW_CLASS" class
        if (!RegisterClassEx(&wnd_class))
        {
            // TODO: Use own logger and return value
            printf("Failed to register window class.\n");
            return false;
        }
    }

    m_id = CreateWindowEx(
        NULL,
        joj_wnd_class_name,
        title.c_str(),
        m_style,
        m_xpos, m_ypos,
        width, height,
        NULL,
        NULL,
        app_id,
        NULL
    );

    if (!m_id)
    {
        // TODO: Use own logger and return value
        printf("Failed to create a window.\n");
        return false;
    }

    if (m_mode == WindowMode::WINDOWED)
    {
        RECT new_rect = { 0, 0, width, height };

        if (!AdjustWindowRectEx(&new_rect,
            GetWindowStyle(m_id),
            GetMenu(m_id) != NULL,
            GetWindowExStyle(m_id)))
        {
            // TODO: Use own logger
            printf("Could not adjust window rect ex.\n");
        }

        m_xpos = (GetSystemMetrics(SM_CXSCREEN) / 2) - ((new_rect.right - new_rect.left) / 2);
        m_ypos = (GetSystemMetrics(SM_CYSCREEN) / 2) - ((new_rect.bottom - new_rect.top) / 2);

        if (!MoveWindow(
            m_id,
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

    m_hdc = GetDC(m_id);

    if (!m_hdc)
    {
        // TODO: Use own logger and return value
        printf("Failed to get device context.\n");
        return false;
    }
    
    if (!GetClientRect(m_id, &m_rect))
    {
        // TODO: Use own logger and return value
        printf("Failed to get client area size.\n");
        return false;
    }
    
    return (m_id ? true : false);
}

b8 joj::Win32Window::create_simple_window()
{
    // TODO: Use own logger and return value
    printf("TODO()!\n");
    return true;
}

void joj::Win32Window::show()
{
    
}

void joj::Win32Window::shutdown()
{
    if (m_hdc)
        ReleaseDC(m_id, m_hdc);
    
    DestroyWindow(m_id);
}

void joj::Win32Window::clear()
{
    
}

void joj::Win32Window::swap_buffers()
{
    
}

void joj::Win32Window::set_on_focus(void(*func)())
{
    on_focus = func;
}

void joj::Win32Window::set_lost_focus(void(*func)())
{
    lost_focus = func;
}

LRESULT CALLBACK joj::Win32Window::WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
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

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
