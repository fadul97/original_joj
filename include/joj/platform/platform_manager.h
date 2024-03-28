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
    enum class BackendRenderer { DX11, GL };

    template<class Twindow, class Tinput>
    class JAPI IPlatformManager
    {
    public:
        IPlatformManager();
        virtual ~IPlatformManager();

        virtual std::unique_ptr<Twindow> create_window(i16 width, i16 height, const char* title, WindowMode mode) = 0;
        virtual std::unique_ptr<Twindow> create_simple_window(i16 width, i16 height, const char* title) = 0;

        virtual std::unique_ptr<Tinput> create_input() = 0;

        virtual std::unique_ptr<GraphicsContext<Twindow>> create_context(std::unique_ptr<Twindow>& window, BackendRenderer backend_renderer) = 0;
        virtual ErrorCode create_window_and_context(std::unique_ptr<Twindow>& window, BackendRenderer backend_renderer) = 0;

        virtual std::unique_ptr<Timer> create_timer() = 0;

        virtual void set_on_focus(void(*func)()) = 0;
        virtual void set_lost_focus(void(*func)()) = 0;

        virtual void set_window_icon(std::unique_ptr<Twindow>& window, i32 count, IconImage& image) = 0;
        virtual void hide_cursor(b8 hide) = 0;
    };
} // namespace joj

#endif // JOJ_PLATFORM_MANAGER_H
