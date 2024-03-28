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
    class JAPI Win32PlatformManager : public IPlatformManager<Win32Window, Win32Input>
    {
    public:
        Win32PlatformManager();
        ~Win32PlatformManager();

        std::unique_ptr<Win32Window> create_window(i16 width, i16 height, const char* title, WindowMode mode) override;
        std::unique_ptr<Win32Window> create_simple_window(i16 width, i16 height, const char* title) override;

        ErrorCode create_context(std::unique_ptr<Win32Window>& window, BackendRenderer backend_renderer) override;
        ErrorCode create_window_and_context(std::unique_ptr<Win32Window>& window, BackendRenderer backend_renderer) override;
        
        std::unique_ptr<Win32Input> create_input() override;

        std::unique_ptr<Timer> create_timer() override;

        ErrorCode make_gl_context_current(std::unique_ptr<Win32Window>& window) override;

        void set_on_focus(void(*func)()) override;
        void set_lost_focus(void(*func)()) override;

        void set_window_icon(std::unique_ptr<Win32Window>& window, i32 count, IconImage& image) override;
        void hide_cursor(b8 hide) override;

    private:
        std::unique_ptr<GraphicsContext<Win32Window>> m_context;
    };

    inline void Win32PlatformManager::hide_cursor(b8 hide)
    { ShowCursor(!hide); }

    inline void Win32PlatformManager::set_on_focus(void(*func)())
    { printf("TODO()!\n"); }

    inline void Win32PlatformManager::set_lost_focus(void(*func)())
    { printf("TODO()!\n"); }
} // namespace joj

#endif // JOJ_WIN32_PLATFORM_MANAGER_H

#endif // JPLATFORM_WINDOWS
