#ifndef JOJ_WINDOW_H
#define JOJ_WINDOW_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <string>

namespace joj
{
    enum class WindowMode { Borderless, Fullscreen };

    class JAPI Window
    {
    public:
        Window(i16 width = 600, i16 height = 400, std::string title = std::string{"Joj Window"});
        virtual ~Window();

    protected:
        i16 width;
        i16 height;
        std::string title;
        b8 running;

        i16 get_width() const;
        i16 get_height() const;
        std::string get_title() const;
        i16 get_xcenter() const;
        i16 get_ycenter() const;
        f32 get_aspect_ratio() const;
        b8 is_running() const;
        
        void set_width(i16 width);
        void set_height(i16 height);
        void set_title(std::string title);
        void close();

        virtual void hide_cursor(b8 hide) = 0;
        
        virtual b8 init() = 0;
        virtual b8 create() = 0;
        virtual void show() = 0;
        virtual void shutdown() = 0;
        virtual void clear() = 0;
        virtual void swap_buffers() = 0;
    };
    
    inline i16 Window::get_width() const
    { return width; }
    
    inline i16 Window::get_height() const
    { return height; }
    
    inline std::string Window::get_title() const
    { return title; }
    
    inline i16 Window::get_xcenter() const
    { return static_cast<i16>(width / 2); }
    
    inline i16 Window::get_ycenter() const
    { return static_cast<i16>(height / 2); }
    
    inline f32 Window::get_aspect_ratio() const
    { return static_cast<f32>(width) / height ; }

    inline b8 Window::is_running() const
    { return running; }
    
    inline void Window::set_width(i16 width)
    { this->width = width; }
    
    inline void Window::set_height(i16 height)
    { this->height = height; }
    
    inline void Window::set_title(std::string title)
    { this->title = title; }

    inline void Window::close()
    { running = false; }
}

#endif // JOJ_WINDOW_H
