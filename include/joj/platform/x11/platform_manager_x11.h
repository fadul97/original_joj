#ifndef JOJ_X11_PLATFORM_MANAGER_H
#define JOJ_X11_PLATFORM_MANAGER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_LINUX

#include "platform/platform_manager.h"
#include "platform/x11/window_x11.h"
#include "platform/x11/input_x11.h"
#include "graphics/opengl/x11/context_gl.h"

namespace joj
{
    class JAPI X11PlatformManager : public PlatformManager<X11Window, X11Input, X11GLContext>
    {
    public:
        X11PlatformManager();
        ~X11PlatformManager();

        b8 init(i16 width = 800, i16 height = 600, std::string title = std::string{ "Joj X11PlatformManager" }) override;
        b8 create_window() override;
        b8 create_simple_window(i16 width = 800, i16 height = 600, std::string title = std::string{ "Joj X11PlatformManager" }) override;
        b8 create_context(BackendRender backend_renderer) override;
        b8 process_events() override;
        void swap_buffers() override;
        void shutdown() override;

        void begin_period() override {}
        void end_period() override {}
        void start_timer() override {}
        void stop_timer() override {}
        f32 reset_timer() override { return 1.0f; }
        
        std::unique_ptr<X11Window>& get_window() override;

        void set_window_icon(i32 count, IconImage& image) override;
    
    private:
        unsigned long delete_msg;
    };

    inline std::unique_ptr<X11Window>& X11PlatformManager::get_window()
	{ return window; }
}

#endif // JOJ_X11_PLATFORM_MANAGER_H

#endif // JPLATFORM_LINUX
