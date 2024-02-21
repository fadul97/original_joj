#include "platform/win32/input_win32.h"

joj::Win32Input::Win32Input()
{
    // ATTENTION: input must be initialized after window creation
    SetWindowLongPtr(GetActiveWindow(), GWLP_WNDPROC, (LONG_PTR)joj::Win32Input::InputProc);
}

joj::Win32Input::~Win32Input()
{
}

LRESULT CALLBACK joj::Win32Input::InputProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    // Key pressed
    case WM_KEYDOWN:
        m_keyboard.keys[wParam] = true;
        return 0;

    // Key released
    case WM_KEYUP:
        m_keyboard.keys[wParam] = false;
        return 0;

    // Mouse movement
    case WM_MOUSEMOVE:
        m_mouse.x = GET_X_LPARAM(lParam);
        m_mouse.y = GET_Y_LPARAM(lParam);
        return 0;

    // Mouse wheel movement
    case WM_MOUSEWHEEL:
        m_mouse.wheel = GET_WHEEL_DELTA_WPARAM(wParam);
        return 0;

    // Left mouse button pressed
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
        m_keyboard.keys[VK_LBUTTON] = true;
        return 0;

    // Middle mouse button pressed
    case WM_MBUTTONDOWN:
    case WM_MBUTTONDBLCLK:
        m_keyboard.keys[VK_MBUTTON] = true;
        return 0;

        // Right mouse button pressed
    case WM_RBUTTONDOWN:
    case WM_RBUTTONDBLCLK:
        m_keyboard.keys[VK_RBUTTON] = true;
        return 0;

    // Left mouse button released
    case WM_LBUTTONUP:
        m_keyboard.keys[VK_LBUTTON] = false;
        return 0;

    // Middle mouse button released
    case WM_MBUTTONUP:
        m_keyboard.keys[VK_MBUTTON] = false;
        return 0;

    // Right mouse button released
    case WM_RBUTTONUP:
        m_keyboard.keys[VK_RBUTTON] = false;
        return 0;
    }

    return CallWindowProc(joj::Win32Window::WinProc, hWnd, msg, wParam, lParam);  
}
