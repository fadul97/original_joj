#ifndef JOJ_WIN32_WINDOW_H
#define JOJ_WIN32_WINDOW_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include "platform/window.h"
#include <windows.h>

namespace joj
{
    struct WindowConfig {
        HWND handle;
        HDC hdc;
        WindowMode window_mode;
        u16 width;
        u16 height;
    };

    class JAPI Win32Window : public Window<WindowConfig>
    {
    public:
        Win32Window();
        ~Win32Window() override;

        void get_window_size(u16 &width, u16 &height) override;
        void get_client_size(u16 &width, u16 &height) override;

        [[nodiscard]] u16 get_width() const override;
        [[nodiscard]] u16 get_height() const override;
        [[nodiscard]] u16 get_xcenter() override;
        [[nodiscard]] u16 get_ycenter() override;
        [[nodiscard]] f32 get_aspect_ratio() const override;

        void set_mode(WindowMode mode) override;
        void set_color(u32 r, u32 g, u32 b) override;
        void set_title(const char* title) override;

        ErrorCode create(i16 width, i16 height, const char* title, WindowMode mode) override;
        void destroy() override;
        void swap_buffers() override;
        
        void set_on_focus(void(*func)()) override;
        void set_lost_focus(void(*func)()) override;

        static LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        
    private:
        WindowConfig m_window_config;
        RECT m_window_rect;
        RECT m_client_rect;
        HICON m_icon;
        HCURSOR m_cursor;
        COLORREF m_color;
        DWORD m_style;

        u16 m_xpos;
        u16 m_ypos;
        u16 m_xcenter;
        u16 m_ycenter;

        static void (*on_focus)();
        static void (*lost_focus)();
    };

    inline u16 Win32Window::get_width() const
    { return m_window_config.width; }

    inline u16 Win32Window::get_height() const
    { return m_window_config.width; }

    inline f32 Win32Window::get_aspect_ratio() const
    { return static_cast<f32>(m_window_config.width) / static_cast<f32>(m_window_config.height); }

    inline void Win32Window::set_title(const char* title)
    {
        SetWindowText(m_window_config.handle, title);
        m_title = title;
    }

}

#endif // JPLATFORM_WINDOWS

#endif // JOJ_WIN32_WINDOW_H
