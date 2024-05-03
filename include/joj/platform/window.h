#ifndef JOJ_WINDOW_H
#define JOJ_WINDOW_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "error_codes.h"

namespace joj
{
    enum class WindowMode { Borderless, Fullscreen, Windowed };

    template<typename TWindowConfig>
    class JAPI Window
    {
    public:
        Window(i16 width, i16 height, const char* title, WindowMode mode);
        virtual ~Window();

        TWindowConfig& get_window_config() const;

        [[nodiscard]] i16 get_width() const;
        [[nodiscard]] i16 get_height() const;
        [[nodiscard]] const char* get_title() const;
        [[nodiscard]] i16 get_xcenter() const;
        [[nodiscard]] i16 get_ycenter() const;
        [[nodiscard]] f32 get_aspect_ratio() const;
        [[nodiscard]] b8 is_running() const;
        [[nodiscard]] WindowMode get_mode() const;
        
        virtual void set_mode(WindowMode mode) = 0;
        virtual void set_color(u32 r, u32 g, u32 b) = 0;
        
        virtual ErrorCode init() = 0;
        virtual ErrorCode create() = 0;
        virtual void show() = 0;
        virtual void destroy() = 0;
        virtual void swap_buffers() = 0;

        virtual void set_on_focus(void(*func)()) = 0;
        virtual void set_lost_focus(void(*func)()) = 0;

    protected:
        i16 m_width;
        i16 m_height;
        const char* m_title;
        b8 m_running;
        WindowMode m_mode;
        TWindowConfig m_window_config;
    };

    template<typename TWindowConfig>
    inline TWindowConfig& Window<TWindowConfig>::get_window_config() const
    { return m_window_config; }

    template<typename TWindowConfig>
    inline i16 Window<TWindowConfig>::get_width() const
    { return m_width; }

    template<typename TWindowConfig>
    inline i16 Window<TWindowConfig>::get_height() const
    { return m_height; }
    
    template<typename TWindowConfig>
    inline const char* Window<TWindowConfig>::get_title() const
    { return m_title; }
    
    template<typename TWindowConfig>
    inline i16 Window<TWindowConfig>::get_xcenter() const
    { return static_cast<i16>(m_width / 2); }
    
    template<typename TWindowConfig>
    inline i16 Window<TWindowConfig>::get_ycenter() const
    { return static_cast<i16>(m_height / 2); }
    
    template<typename TWindowConfig>
    inline f32 Window<TWindowConfig>::get_aspect_ratio() const
    { return static_cast<f32>(m_width) / m_height ; }

    template<typename TWindowConfig>
    inline b8 Window<TWindowConfig>::is_running() const
    { return m_running; }

    template<typename TWindowConfig>
    inline WindowMode Window<TWindowConfig>::get_mode() const
    { return m_mode; }
}

#endif // JOJ_WINDOW_H
