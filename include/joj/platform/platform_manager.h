#ifndef JOJ_PLATFORM_MANAGER_H
#define JOJ_PLATFORM_MANAGER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <string>
#include "window.h"
#include "input.h"
#include "timer.h"
#include "context/context.h"
#include <memory>
#include "icon_image.h"
#include "error.h"

namespace joj
{
    struct Point
    {
        i16 x;
        i16 y;
    };

    enum class BackendRenderer { DX11, GL };

    template<class Twindow, class Tinput>
    class JAPI PlatformManager
    {
    public:
        PlatformManager();
        virtual ~PlatformManager();

        // Window related methods
        virtual ErrorCode create_window(i16 width, i16 height, const char* title, WindowMode mode) = 0;
        virtual std::unique_ptr<Twindow> create_simple_window(i16 width, i16 height, const char* title) = 0;

        virtual void set_on_focus(void(*func)()) = 0;
        virtual void set_lost_focus(void(*func)()) = 0;
        
        virtual void set_window_icon(std::unique_ptr<Twindow>& window, i32 count, IconImage& image) = 0;
        virtual void hide_cursor(b8 hide) = 0;
        
        virtual Point get_cursor_position() = 0;
        virtual void set_cursor_position(i16 x, i16 y) = 0;
        
        virtual std::unique_ptr<Twindow>& get_window() = 0;
        virtual void change_window_procedure(std::unique_ptr<Twindow>& window, void* func) = 0;
        virtual void set_window_title(std::unique_ptr<Twindow>& window, const char* title) = 0;

        virtual void close_window();
                
        i16 get_xcenter() const;
        i16 get_ycenter() const;
        f32 get_aspect_ratio() const;

        // Input related methods
        virtual ErrorCode create_input() = 0;

        b8 is_key_down(u32 key) const;
        b8 is_key_pressed(u32 key) const;
        b8 is_key_up(u32 key) const;

        b8 is_button_down(Buttons button) const;
        b8 is_button_up(Buttons button) const;

        i16 get_xmouse() const;
        i16 get_ymouse() const;
        i16 get_mouse_wheel() const;

        virtual b8 process_events() = 0;

        // Context related methods
        virtual ErrorCode create_context(std::unique_ptr<Twindow>& window, BackendRenderer backend_renderer) = 0;
        virtual ErrorCode create_window_and_context(std::unique_ptr<Twindow>& window, BackendRenderer backend_renderer) = 0;
        virtual ErrorCode make_gl_context_current(std::unique_ptr<Twindow>& window) = 0;

        // Timer related methods
        virtual ErrorCode create_timer() = 0;

        virtual void begin_period() = 0;
        virtual void end_period() = 0;
        virtual void start_timer() = 0;
        virtual void stop_timer() = 0;
        virtual f32 reset_timer() = 0;

        // State related methods
        virtual void shutdown() = 0;

    protected:
        std::unique_ptr<Twindow> m_window;
        std::unique_ptr<Tinput> m_input;
    };

    template<class Twindow, class Tinput>
    inline i16 PlatformManager<Twindow, Tinput>::get_xcenter() const
    { return m_window->get_xcenter(); }

    template<class Twindow, class Tinput>
    inline i16 PlatformManager<Twindow, Tinput>::get_ycenter() const
    { return m_window->get_ycenter(); }

    template<class Twindow, class Tinput>
    inline f32 PlatformManager<Twindow, Tinput>::get_aspect_ratio() const
    { return m_window->get_aspect_ratio(); }

    template<class Twindow, class Tinput>
    inline void PlatformManager<Twindow, Tinput>::close_window()
    { m_window->stop_running(); }

    template<class Twindow, class Tinput>
    inline b8 PlatformManager<Twindow, Tinput>::is_key_down(u32 key) const
    { return m_input->is_key_down(key); }

    template<class Twindow, class Tinput>
    inline b8 PlatformManager<Twindow, Tinput>::is_key_pressed(u32 key) const
    { return m_input->is_key_pressed(key); }

    template<class Twindow, class Tinput>
    inline b8 PlatformManager<Twindow, Tinput>::is_key_up(u32 key) const
    { return m_input->is_key_up(key); }

    template<class Twindow, class Tinput>
    inline b8 PlatformManager<Twindow, Tinput>::is_button_down(Buttons button) const
    { return m_input->is_button_down(button); }

    template<class Twindow, class Tinput>
    inline b8 PlatformManager<Twindow, Tinput>::is_button_up(Buttons button) const
    { return m_input->is_button_up(button); }

    template<class Twindow, class Tinput>
    inline i16 PlatformManager<Twindow, Tinput>::get_xmouse() const
    { return m_input->get_xmouse(); }

    template<class Twindow, class Tinput>
    inline i16 PlatformManager<Twindow, Tinput>::get_ymouse() const
    { return m_input->get_ymouse(); }

    template<class Twindow, class Tinput>
    inline i16 PlatformManager<Twindow, Tinput>::get_mouse_wheel() const
    { return m_input->get_mouse_wheel(); }

} // namespace joj

#endif // JOJ_PLATFORM_MANAGER_H
