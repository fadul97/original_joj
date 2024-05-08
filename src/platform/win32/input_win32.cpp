#include "platform/win32/input_win32.h"

#if JPLATFORM_WINDOWS

#include <windowsx.h>

joj::Win32Input::Win32Input()
{
    // ATTENTION: input must be initialized after window creation
    SetWindowLongPtr(GetActiveWindow(), GWLP_WNDPROC, (LONG_PTR)InputProc);
}

joj::Win32Input::~Win32Input() = default;

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
        m_mouse.buttons[BUTTON_LEFT] = true;
        return 0;

    // Middle mouse button pressed
    case WM_MBUTTONDOWN:
    case WM_MBUTTONDBLCLK:
        m_keyboard.keys[VK_MBUTTON] = true;
        m_mouse.buttons[BUTTON_MIDDLE] = true;
        return 0;

        // Right mouse button pressed
    case WM_RBUTTONDOWN:
    case WM_RBUTTONDBLCLK:
        m_keyboard.keys[VK_RBUTTON] = true;
        m_mouse.buttons[BUTTON_RIGHT] = true;
        return 0;

    // Left mouse button released
    case WM_LBUTTONUP:
        m_keyboard.keys[VK_LBUTTON] = false;
        m_mouse.buttons[BUTTON_LEFT] = false;
        return 0;

    // Middle mouse button released
    case WM_MBUTTONUP:
        m_keyboard.keys[VK_MBUTTON] = false;
        m_mouse.buttons[BUTTON_MIDDLE] = false;
        return 0;

    // Right mouse button released
    case WM_RBUTTONUP:
        m_keyboard.keys[VK_RBUTTON] = false;
        m_mouse.buttons[BUTTON_RIGHT] = false;
        return 0;

    default:
        break;
    }

    return CallWindowProc(Win32Window::WinProc, hWnd, msg, wParam, lParam);
}

#endif // JPLATFORM_WINDOWS
