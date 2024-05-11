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

    class JAPI Win32Window //: public Window<WindowConfig>
    {
    public:
        Win32Window();
        ~Win32Window();

        WindowConfig& get_window_config();

        void get_window_size(u16 &width, u16 &height);
        void get_client_size(u16 &width, u16 &height);

        [[nodiscard]] u16 get_width() const;
        [[nodiscard]] u16 get_height() const;
        [[nodiscard]] u16 get_xcenter();
        [[nodiscard]] u16 get_ycenter();
        [[nodiscard]] f32 get_aspect_ratio() const;

        void set_mode(WindowMode mode);
        void set_color(u32 r, u32 g, u32 b);
        void set_title(const char* title) const;

        ErrorCode create(i16 width, i16 height, const char* title, WindowMode mode);
        void destroy();
        void swap_buffers() const;
        
        void set_on_focus(void(*func)());
        void set_lost_focus(void(*func)());

        static LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        
    private:
        WindowConfig m_window_config{};
        RECT m_window_rect{};
        RECT m_client_rect{};
        HICON m_icon;
        HCURSOR m_cursor;
        COLORREF m_color;
        DWORD m_style;

        u16 m_xcenter{};
        u16 m_ycenter{};

        static void (*on_focus)();
        static void (*lost_focus)();
    };

    inline WindowConfig& Win32Window::get_window_config()
    { return m_window_config; }

    inline u16 Win32Window::get_width() const
    { return m_window_config.width; }

    inline u16 Win32Window::get_height() const
    { return m_window_config.width; }

    inline f32 Win32Window::get_aspect_ratio() const
    { return static_cast<f32>(m_window_config.width) / static_cast<f32>(m_window_config.height); }

    inline void Win32Window::set_title(const char* title) const
    {
        SetWindowText(m_window_config.handle, title);
        // m_title = title;
    }

}

#endif // JPLATFORM_WINDOWS

#endif // JOJ_WIN32_WINDOW_H
