#ifndef JOJ_WIN32_PLATFORM_MANAGER_H
#define JOJ_WIN32_PLATFORM_MANAGER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include "platform/platform_manager.h"
#include "platform/win32/window_win32.h"
#include "platform/win32/input_win32.h"
#include "platform/context/opengl/win32/context_gl.h"
#include "platform/context/dx11/context_dx11.h"
#include "platform/win32/timer_win32.h"

namespace joj
{
    class JAPI Win32PlatformManager : public PlatformManager<Win32Window, Win32Input>
    {
    public:
        Win32PlatformManager();
        ~Win32PlatformManager();

        // Window related methods
        ErrorCode create_window(i16 width, i16 height, const char* title, WindowMode mode) override;
        std::unique_ptr<Win32Window> create_simple_window(i16 width, i16 height, const char* title) override;

        void set_on_focus(void(*func)()) override;
        void set_lost_focus(void(*func)()) override;

        void set_window_icon(std::unique_ptr<Win32Window>& window, i32 count, IconImage& image) override;
        void hide_cursor(b8 hide) override;

        Point get_cursor_position() override;
        void set_cursor_position(i16 x, i16 y) override;

        std::unique_ptr<Win32Window>& get_window() override;

        void change_window_procedure(std::unique_ptr<Win32Window>& window, void* func) override;
        void set_window_title(std::unique_ptr<Win32Window>& window, const char* title) override;
        
        // Input related methods
        ErrorCode create_input() override;

        b8 process_events() override;

        // Context related methods
        ErrorCode create_context(std::unique_ptr<Win32Window>& window, BackendRenderer backend_renderer) override;
        ErrorCode create_window_and_context(std::unique_ptr<Win32Window>& window, BackendRenderer backend_renderer) override;
        ErrorCode make_gl_context_current(std::unique_ptr<Win32Window>& window) override;
        
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
        std::unique_ptr<GraphicsContext<Win32Window>> m_context;
        std::unique_ptr<Win32Timer> m_timer;
    };

    inline void Win32PlatformManager::hide_cursor(b8 hide)
    { ShowCursor(!hide); }

    inline std::unique_ptr<Win32Window>& Win32PlatformManager::get_window()
    { return m_window; }

    inline void  Win32PlatformManager::set_on_focus(void(*func)())
    { m_window->set_on_focus(func); }
    
    inline void  Win32PlatformManager::set_lost_focus(void(*func)())
    { m_window->set_lost_focus(func); }

    inline void Win32PlatformManager::begin_period()
	{ m_timer->time_begin_period(); }

	// Return sleep resolution to original value
	inline void Win32PlatformManager::end_period()
	{ m_timer->time_end_period(); }

	// Start/resume counting time
	inline void Win32PlatformManager::start_timer()
	{ m_timer->start(); }

	// Stop counting time
	inline void Win32PlatformManager::stop_timer()
	{ m_timer->stop(); }

	// Restarts timer counter and returns elapsed time
	inline f32 Win32PlatformManager::reset_timer()
	{ return m_timer->reset(); }

    inline Point Win32PlatformManager::get_cursor_position()
    {
        POINT cursor_pos;
        GetCursorPos(&cursor_pos);
        return Point{static_cast<i16>(cursor_pos.x), static_cast<i16>(cursor_pos.y) };
    }

    inline void Win32PlatformManager::set_cursor_position(i16 x, i16 y)
    { SetCursorPos(x, y); }
} // namespace joj

#endif // JOJ_WIN32_PLATFORM_MANAGER_H

#endif // JPLATFORM_WINDOWS
