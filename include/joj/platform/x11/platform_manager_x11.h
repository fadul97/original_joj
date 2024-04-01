#ifndef JOJ_X11_PLATFORM_MANAGER_H
#define JOJ_X11_PLATFORM_MANAGER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_LINUX

#include "platform/platform_manager.h"
#include "platform/x11/window_x11.h"
#include "platform/x11/input_x11.h"
#include "platform/x11/timer_x11.h"
#include "platform/context/opengl/x11/context_gl.h"
#include <stdio.h>
#include "error.h"

namespace joj
{
    class JAPI X11PlatformManager : public PlatformManager<X11Window, X11Input>
    {
    public:
        X11PlatformManager();
        ~X11PlatformManager();

        // Window related methods
        ErrorCode create_window(i16 width, i16 height, const char* title, WindowMode mode) override;
        std::unique_ptr<X11Window> create_simple_window(i16 width, i16 height, const char* title) override;

        void set_on_focus(void(*func)()) override;
        void set_lost_focus(void(*func)()) override;

        void set_window_icon(std::unique_ptr<X11Window>& window, i32 count, IconImage& image) override;
        void hide_cursor(b8 hide) override;

        Point get_cursor_position() override;
        void set_cursor_position(i16 x, i16 y) override;

        std::unique_ptr<X11Window>& get_window() override;

        void change_window_procedure(std::unique_ptr<X11Window>& window, void* func) override;
        void set_window_title(std::unique_ptr<X11Window>& window, const char* title) override;

        void swap_buffers();
        
        // Input related methods
        ErrorCode create_input() override;

        b8 process_events() override;

        // Context related methods
        ErrorCode create_context(std::unique_ptr<X11Window>& window, BackendRenderer backend_renderer) override;
        ErrorCode create_window_and_context(std::unique_ptr<X11Window>& window, BackendRenderer backend_renderer) override;
        ErrorCode make_gl_context_current(std::unique_ptr<X11Window>& window) override;
        
        // Timer related methods
        ErrorCode create_timer() override;

        void begin_period() override;
        void end_period() override;
        void start_timer() override;
        void stop_timer() override;
        f32 reset_timer() override;

        // State related methods
        void shutdown() override;
    
    private:
        unsigned long delete_msg;

        std::unique_ptr<X11Timer> m_timer;
        std::unique_ptr<X11GLContext> m_context;
    };

    inline void X11PlatformManager::hide_cursor(b8 hide)
    { printf("TODO()!\n"); }

    inline std::unique_ptr<X11Window>& X11PlatformManager::get_window()
	{ return m_window; }

    inline void  X11PlatformManager::set_on_focus(void(*func)())
    { m_window->set_on_focus(func); }
    
    inline void  X11PlatformManager::set_lost_focus(void(*func)())
    { m_window->set_lost_focus(func); }

    inline void X11PlatformManager::begin_period()
	{ m_timer->time_begin_period(); }

	// Return sleep resolution to original value
	inline void X11PlatformManager::end_period()
	{ m_timer->time_end_period(); }

	// Start/resume counting time
	inline void X11PlatformManager::start_timer()
	{ m_timer->start(); }

	// Stop counting time
	inline void X11PlatformManager::stop_timer()
	{ m_timer->stop(); }

	// Restarts timer counter and returns elapsed time
	inline f32 X11PlatformManager::reset_timer()
	{ return m_timer->reset(); }

    inline Point X11PlatformManager::get_cursor_position()
    {
        return Point{static_cast<i16>(m_input->get_xmouse()), static_cast<i16>(m_input->get_ymouse()) };
    }

    inline void X11PlatformManager::set_cursor_position(i16 x, i16 y)
    { printf("TODO()!\n"); }
}

#endif // JOJ_X11_PLATFORM_MANAGER_H

#endif // JPLATFORM_LINUX
