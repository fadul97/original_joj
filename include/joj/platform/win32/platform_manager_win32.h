#ifndef JOJ_WIN32_PLATFORM_MANAGER_H
#define JOJ_WIN32_PLATFORM_MANAGER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include "platform/platform_manager.h"
#include "platform/win32/window_win32.h"
#include "platform/win32/input_win32.h"
#include "graphics/opengl/win32/context_gl.h"
#include "graphics/dx11/context_dx11.h"
#include "platform/win32/timer_win32.h"

namespace joj
{
    class JAPI Win32PlatformManager : public IPlatformManager<Win32Window>
    {
    public:
        Win32PlatformManager();
        ~Win32PlatformManager();

        b8 is_running() const override;

        b8 is_key_down(u32 key) const override;
        b8 is_key_pressed(u32 key) const override;
        b8 is_key_up(u32 key) const override;

        b8 is_button_down(Buttons button) const override;
        b8 is_button_up(Buttons button) const override;

        i16 get_xmouse() const override;
        i16 get_ymouse() const override;
        i16 get_mouse_wheel() const override;

        void hide_cursor(b8 hide) override;

        void set_window_mode(WindowMode mode) override;
        void set_window_color(i32 r, i32 g, i32 b) override;
        void set_on_focus(void(*func)()) override;
        void set_lost_focus(void(*func)()) override;

        std::unique_ptr<Win32Window>& get_window() override;

        void close_window() override;

        ErrorCode init(i16 width, i16 height, std::string title, WindowMode mode) override;
        ErrorCode create_window() override;
        ErrorCode create_simple_window(i16 width, i16 height, std::string title, WindowMode mode) override;
        ErrorCode create_context(BackendRenderer backend_renderer) override;
        ErrorCode create_window_and_context(BackendRenderer backend_renderer) override;
        b8 process_events() override;
        void swap_buffers() override;
        void shutdown() override;

        f32 get_aspect_ratio() const override;
        f32 get_xcenter() const override;
        f32 get_ycenter() const override;

        void begin_period() override;
        void end_period() override;
        void start_timer() override;
        void stop_timer() override;
        f32 reset_timer() override;

        void set_window_icon(i32 count, IconImage& image) override;

    private:
        MSG msg;
        std::unique_ptr<Win32Window> m_window;
        std::unique_ptr<Win32Input> m_input;
        std::unique_ptr<DX11Context> m_dx11_context;
        std::unique_ptr<Win32GLContext> m_gl_context;
        std::unique_ptr<Win32Timer> m_timer;
    };

    inline std::unique_ptr<Win32Window>& Win32PlatformManager::get_window()
	{ return m_window; }
    
    inline b8 Win32PlatformManager::is_running() const
	{ return (m_window->is_running() && msg.message != WM_QUIT); }

    inline b8 Win32PlatformManager::is_key_down(u32 key) const
    { return m_input->is_key_down(key); }

    inline b8 Win32PlatformManager::is_key_pressed(u32 key) const
    { return m_input->is_key_pressed(key); }

    inline b8 Win32PlatformManager::is_key_up(u32 key) const
    { return m_input->is_key_up(key); }

    inline b8 Win32PlatformManager::is_button_down(Buttons button) const
    { return m_input->is_button_down(button); }

    inline b8 Win32PlatformManager::is_button_up(Buttons button) const
    { return m_input->is_button_up(button); }

    inline i16 Win32PlatformManager::get_xmouse() const
    { return m_input->get_xmouse(); }

    inline i16 Win32PlatformManager::get_ymouse() const
    { return m_input->get_ymouse(); }

    inline i16 Win32PlatformManager::get_mouse_wheel() const
    { return m_input->get_mouse_wheel(); }

    inline void Win32PlatformManager::hide_cursor(b8 hide)
    { ShowCursor(!hide); }

    inline void Win32PlatformManager::set_window_mode(WindowMode mode)
    { m_window->set_mode(mode); }

    inline void Win32PlatformManager::set_window_color(i32 r, i32 g, i32 b)
    { m_window->set_color(r, g, b); }

    // Set function to be executed when wubdiw regains focus
	inline void Win32PlatformManager::set_on_focus(void(*func)())
	{ m_window->set_on_focus(func); }

	// Set function to be executed when wubdiw loses focus
	inline void Win32PlatformManager::set_lost_focus(void(*func)())
	{ m_window->set_lost_focus(func); }

    inline void Win32PlatformManager::close_window()
    { m_window->close(); }

    inline f32 Win32PlatformManager::get_aspect_ratio() const
    { return m_window->get_aspect_ratio(); }

    inline f32 Win32PlatformManager::get_xcenter() const
    { return m_window->get_xcenter(); }

    inline f32 Win32PlatformManager::get_ycenter() const
    { return m_window->get_ycenter(); }

    // Adjust sleep resolution to 1 millisecond
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
} // namespace joj

#endif // JOJ_WIN32_PLATFORM_MANAGER_H

#endif // JPLATFORM_WINDOWS
