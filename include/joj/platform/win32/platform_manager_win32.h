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
    class JAPI Win32PlatformManager : public PlatformManager<Win32Window, Win32Input, Win32GLContext>
    {
    public:
        Win32PlatformManager();
        ~Win32PlatformManager();
        
        b8 is_running() const;

        b8 init(i16 width = 800, i16 height = 600, std::string title = std::string{ "Joj Win32PlatformManager" }) override;
        b8 create_window() override;
        b8 create_simple_window(i16 width = 800, i16 height = 600, std::string title = std::string{ "Joj Win32PlatformManager" }) override;
        b8 create_context(BackendRender backend_renderer) override;
        b8 process_events() override;
        void swap_buffers() override;
        void shutdown() override;

        void begin_period();    // Adjust sleep resolution to 1 millisecond
        void end_period();      // Return sleep resolution to original value
        void start_timer();     // Start/resume counting time
        void stop_timer();      // Stop counting time
        f32 reset_timer();      // Restarts timer counter and returns elapsed time

        void set_on_focus(void(*func)());   // Set function to be executed when wubdiw regains focus
        void set_lost_focus(void(*func)());	// Set function to be executed when wubdiw loses focus

        std::unique_ptr<Win32Window>& get_window() override;

        void set_window_icon(i32 count, IconImage& image) override;

    private:
        MSG msg;
        std::unique_ptr<DX11Context> m_dx11_context;
        std::unique_ptr<Win32Timer> m_timer;
    };

    inline std::unique_ptr<Win32Window>& Win32PlatformManager::get_window()
	{ return window; }
    
    inline b8 Win32PlatformManager::is_running() const
	{ return (window->is_running() && msg.message != WM_QUIT); }

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

    // Set function to be executed when wubdiw regains focus
	inline void Win32PlatformManager::set_on_focus(void(*func)())
	{ window->set_on_focus(func); }

	// Set function to be executed when wubdiw loses focus
	inline void Win32PlatformManager::set_lost_focus(void(*func)())
	{ window->set_lost_focus(func); }
}

#endif // JOJ_WIN32_PLATFORM_MANAGER_H

#endif // JPLATFORM_WINDOWS