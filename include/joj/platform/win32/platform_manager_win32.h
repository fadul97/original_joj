#ifndef JOJ_WIN32_PLATFORM_MANAGER_H
#define JOJ_WIN32_PLATFORM_MANAGER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include "platform/platform_manager.h"
#include "platform/win32/window_win32.h"
#include "platform/win32/input_win32.h"
#include "graphics/opengl/win32/context_gl.h"

namespace joj
{
    class JAPI Win32PlatformManager : public PlatformManager<Win32Window, Win32Input, Win32GLContext>
    {
    public:
        Win32PlatformManager();
        ~Win32PlatformManager();

        b8 init(i16 width = 800, i16 height = 600, std::string title = std::string{ "Joj Win32PlatformManager" }) override;
        b8 create_window() override;
        b8 create_simple_window(i16 width = 800, i16 height = 600, std::string title = std::string{ "Joj Win32PlatformManager" }) override;
        void process_events() override;
        void swap_buffers() override;
        void shutdown() override;

        std::unique_ptr<Win32Window>& get_window() override;

        void set_window_icon(i32 count, IconImage& image) override;
    
    private:
        MSG msg;
    };

    inline std::unique_ptr<Win32Window>& Win32PlatformManager::get_window()
	{ return window; }
}

#endif // JOJ_WIN32_PLATFORM_MANAGER_H

#endif // JPLATFORM_WINDOWS
