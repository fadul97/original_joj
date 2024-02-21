#ifndef JOJ_WINDOW_H
#define JOJ_WINDOW_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <string>

namespace joj
{
    enum class WindowMode { Borderless, Fullscreen };

    template<typename Tid>
    class JAPI Window
    {
    public:
        Window(i16 width = 600, i16 height = 400, std::string title = std::string{"Joj Window"});
        virtual ~Window();

        virtual void* get_display() const = 0;
        virtual Tid get_id() const = 0;
        virtual i32 get_screen_id() const = 0;
        virtual void* get_visual() const = 0;
        virtual void set_visual(void* visual) = 0;

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
        virtual b8 create_simple_window() = 0;
        virtual void show() = 0;
        virtual void shutdown() = 0;
        virtual void clear() = 0;
        virtual void swap_buffers() = 0;

    protected:
        i16 width;
        i16 height;
        std::string title;
        b8 running;
    };
    
    template<typename Tid>
    inline i16 Window<Tid>::get_width() const
    { return width; }
    
    template<typename Tid>
    inline i16 Window<Tid>::get_height() const
    { return height; }
    
    template<typename Tid>
    inline std::string Window<Tid>::get_title() const
    { return title; }
    
    template<typename Tid>
    inline i16 Window<Tid>::get_xcenter() const
    { return static_cast<i16>(width / 2); }
    
    template<typename Tid>
    inline i16 Window<Tid>::get_ycenter() const
    { return static_cast<i16>(height / 2); }
    
    template<typename Tid>
    inline f32 Window<Tid>::get_aspect_ratio() const
    { return static_cast<f32>(width) / height ; }

    template<typename Tid>
    inline b8 Window<Tid>::is_running() const
    { return running; }
    
    template<typename Tid>
    inline void Window<Tid>::set_width(i16 width)
    { this->width = width; }
    
    template<typename Tid>
    inline void Window<Tid>::set_height(i16 height)
    { this->height = height; }
    
    template<typename Tid>
    inline void Window<Tid>::set_title(std::string title)
    { this->title = title; }

    template<typename Tid>
    inline void Window<Tid>::close()
    { running = false; }
}

#endif // JOJ_WINDOW_H
