#ifndef JOJ_PLATFORM_MANAGER_H
#define JOJ_PLATFORM_MANAGER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <string>
#include "window.h"
#include "input.h"
#include "graphics/context.h"
#include <memory>
#include "icon_image.h"
#include "error.h"

namespace joj
{
    template<class Twindow>
    class JAPI IPlatformManager
    {
    public:
        IPlatformManager();
        virtual ~IPlatformManager();

        virtual b8 is_running() const = 0;

        virtual b8 is_key_down(u32 key) const = 0;
        virtual b8 is_key_pressed(u32 key) const = 0;
        virtual b8 is_key_up(u32 key) const = 0;

        virtual b8 is_button_down(Buttons button) const = 0;
        virtual b8 is_button_up(Buttons button) const = 0;

        virtual i16 get_xmouse() const = 0;
        virtual i16 get_ymouse() const = 0;
        virtual i16 get_mouse_wheel() const = 0;

        virtual void hide_cursor(b8 hide) = 0;

        virtual std::unique_ptr<Twindow>& get_window() = 0;

        virtual void set_window_mode(WindowMode mode) = 0;
        virtual void set_window_color(i32 r, i32 g, i32 b) = 0;
        virtual void set_on_focus(void(*func)()) = 0;
        virtual void set_lost_focus(void(*func)()) = 0;
        virtual void close_window() = 0;

        virtual ErrorCode init(i16 width, i16 height, std::string title, WindowMode mode) = 0;
        virtual ErrorCode create_window() = 0;
        virtual ErrorCode create_simple_window(i16 width, i16 height, std::string title, WindowMode mode) = 0;
        virtual ErrorCode create_context(BackendRenderer backend_renderer) = 0;
        virtual ErrorCode create_window_and_context(BackendRenderer backend_renderer) = 0;
        virtual b8 process_events() = 0;
        virtual void swap_buffers() = 0;
        virtual void shutdown() = 0;

        virtual f32 get_aspect_ratio() const = 0;
        virtual f32 get_xcenter() const = 0;
        virtual f32 get_ycenter() const = 0;

        virtual void begin_period() = 0;
        virtual void end_period() = 0;
        virtual void start_timer() = 0;
        virtual void stop_timer() = 0;
        virtual f32 reset_timer() = 0;

        virtual void set_window_icon(i32 count, IconImage& image) = 0;
    };
} // namespace joj

#endif // JOJ_PLATFORM_MANAGER_H
