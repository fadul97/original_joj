#ifndef JOJ_X11_WINDOW_H
#define JOJ_X11_WINDOW_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_LINUX

#include "platform/window.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace joj
{
    class JAPI X11Window : public Window
    {
    public:
        X11Window(i16 width = 600, i16 height = 400, std::string title = std::string{"Joj X11Window"});
        ~X11Window() override;

        void* get_display() const override;
        u32 get_id() const override;
        Screen* get_screen() const;
        i32 get_screen_id() const override;
        void* get_visual() const override;
        void set_visual(void* visual) override;
        XSetWindowAttributes get_wnd_attribs() const;

        void hide_cursor(b8 hide) override;
        b8 init() override;
        b8 create() override;
        b8 create_simple_window() override;
        void show() override;
        void shutdown() override;
        void clear() override;
        void swap_buffers() override;
        
    private:
        Display* display;
        u32 id;
        Screen* screen;
        i32 screen_id;
        XSetWindowAttributes wnd_attribs;
        XVisualInfo* visual;
        unsigned long delete_msg;
        GC gc;
    };

    inline void* X11Window::get_display() const
    { return static_cast<void*>(display); }
    
    inline u32 X11Window::get_id() const
    { return id; }
    
    inline Screen* X11Window::get_screen() const
    { return screen; }
    
    inline i32 X11Window::get_screen_id() const
    { return screen_id; }

    inline void* X11Window::get_visual() const
    { return static_cast<void*>(visual); }

    inline void X11Window::set_visual(void* visual)
    { this->visual = static_cast<XVisualInfo*>(visual); }
    
    inline XSetWindowAttributes X11Window::get_wnd_attribs() const
    { return wnd_attribs; }
}

#endif // JPLATFORM_LINUX

#endif // JOJ_X11_WINDOW_H
