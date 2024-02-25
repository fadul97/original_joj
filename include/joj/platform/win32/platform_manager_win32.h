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
        void process_events() override;
        void swap_buffers() override;
        void shutdown() override;

        std::unique_ptr<Win32Window>& get_window() override;

        void set_window_icon(i32 count, IconImage& image) override;
    
    private:
        MSG msg;
        std::unique_ptr<DX11Context> m_dx11_context;
    };

    inline std::unique_ptr<Win32Window>& Win32PlatformManager::get_window()
	{ return window; }
    
    inline b8 Win32PlatformManager::is_running() const
	{ return (window->is_running() && msg.message != WM_QUIT); }
}

#endif // JOJ_WIN32_PLATFORM_MANAGER_H

#endif // JPLATFORM_WINDOWS
