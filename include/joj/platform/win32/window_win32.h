#ifndef JOJ_WIN32_WINDOW_H
#define JOJ_WIN32_WINDOW_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include "platform/window.h"
#include <windows.h>
#include <windowsx.h>

namespace joj
{
    class JAPI Win32Window : public Window<HWND>
    {
    public:
        Win32Window(i16 width = 600, i16 height = 400, std::string title = std::string{"Joj Win32Window"});
        ~Win32Window() override;

        HWND get_id() const override;
        HDC get_device_context() const;
        COLORREF get_color() const;
        
        void hide_cursor(b8 hide) override;
        void set_mode(WindowMode mode) override;
        void set_color(u32 r, u32 g, u32 b) override;

        void close() override;
        
        b8 init() override;
        b8 create() override;
        b8 create_simple_window() override;
        void show() override;
        void shutdown() override;
        void clear() override;
        void swap_buffers() override;
        
        void set_on_focus(void(*func)()) override;
        void set_lost_focus(void(*func)()) override;

        static LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        
    private:
        HWND m_id;
        HDC m_hdc;
        RECT m_rect;
        HICON m_icon;
        HCURSOR m_cursor;
        COLORREF m_color;
        DWORD m_style;
        WindowMode m_mode;

        u16 m_xpos;
        u16 m_ypos;
        u16 m_xcenter;
        u16 m_ycenter;

        static void (*on_focus)();
        static void (*lost_focus)();
    };

    inline HWND Win32Window::get_id() const
    { return m_id; }
    
    inline HDC Win32Window::get_device_context() const
    { return m_hdc; }
    
    inline COLORREF Win32Window::get_color() const
    { return m_color; }
    
    inline void Win32Window::close()
    { running = false; DestroyWindow(m_id); }
}

#endif // JPLATFORM_WINDOWS

#endif // JOJ_WIN32_WINDOW_H
