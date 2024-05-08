#ifndef JOJ_WINDOW_H
#define JOJ_WINDOW_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "error_code.h"

namespace joj
{
    enum class WindowMode { Borderless, Fullscreen, Windowed };

    template<typename TWindowConfig>
    class JAPI Window
    {
    public:
        Window();
        virtual ~Window();

        TWindowConfig& get_window_config();

        virtual void get_window_size(u16& width, u16& height) = 0;
        virtual void get_client_size(u16& width, u16& height) = 0;

        [[nodiscard]] virtual u16 get_width() const = 0;
        [[nodiscard]] virtual u16 get_height() const = 0;
        [[nodiscard]] const char* get_title() const;
        [[nodiscard]] virtual u16 get_xcenter() = 0;
        [[nodiscard]] virtual u16 get_ycenter() = 0;
        [[nodiscard]] virtual f32 get_aspect_ratio() const = 0;
        [[nodiscard]] b8 is_running() const;

        void stop_running();
        
        virtual void set_mode(WindowMode mode) = 0;
        virtual void set_color(u32 r, u32 g, u32 b) = 0;
        virtual void set_title(const char* title) = 0;

        virtual ErrorCode create(i16 width, i16 height, const char* title, WindowMode mode) = 0;
        virtual void destroy() = 0;
        virtual void swap_buffers() = 0;

        virtual void set_on_focus(void(*func)()) = 0;
        virtual void set_lost_focus(void(*func)()) = 0;

    protected:
        const char* m_title;
        b8 m_running;
        TWindowConfig m_window_config;
    };

    template<typename TWindowConfig>
    inline TWindowConfig& Window<TWindowConfig>::get_window_config()
    { return m_window_config; }

    template<typename TWindowConfig>
    inline const char* Window<TWindowConfig>::get_title() const
    { return m_title; }
    
    template<typename TWindowConfig>
    inline b8 Window<TWindowConfig>::is_running() const
    { return m_running; }

    template<typename TWindowConfig>
    inline void Window<TWindowConfig>::stop_running()
    { m_running = false; }
}

#endif // JOJ_WINDOW_H
