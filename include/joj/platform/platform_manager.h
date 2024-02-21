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

namespace joj
{
    template<class Twindow, class Tinput, class Tcontext>
    class JAPI PlatformManager
    {
    public:
        PlatformManager();
        virtual ~PlatformManager();

        b8 is_running() const;

        b8 is_key_down(Keys key) const;
        b8 is_key_pressed(Keys key) const;
        b8 is_key_up(Keys key) const;

        i16 get_xmouse() const;
        i16 get_ymouse() const;
        i16 get_mouse_wheel() const;

        virtual std::unique_ptr<Twindow>& get_window();

        void close_window();

        virtual b8 init(i16 width = 800, i16 height = 600, std::string title = std::string{ "Joj PlatformManager" }) = 0;
        virtual b8 create_window() = 0;
        virtual b8 create_simple_window(i16 width = 800, i16 height = 600, std::string title = std::string{ "Joj PlatformManager" }) = 0;
        virtual void process_events() = 0;
        virtual void swap_buffers() = 0;
        virtual void shutdown() = 0;

        virtual void set_window_icon(i32 count, IconImage& image) = 0;

    protected:  
        std::unique_ptr<Twindow> window;
        std::unique_ptr<Tinput> input;
        std::unique_ptr<Tcontext> context;
    };

    template<class Twindow, class Tinput, class Tcontext>
    inline b8 PlatformManager<Twindow, Tinput, Tcontext>::is_running() const
    { return window->is_running(); }

    template<class Twindow, class Tinput, class Tcontext>
    inline b8 PlatformManager<Twindow, Tinput, Tcontext>::is_key_down(Keys key) const
    { return input->is_key_down(key); }

    template<class Twindow, class Tinput, class Tcontext>
    inline b8 PlatformManager<Twindow, Tinput, Tcontext>::is_key_pressed(Keys key) const
    { return input->is_key_pressed(key); }

    template<class Twindow, class Tinput, class Tcontext>
    inline b8 PlatformManager<Twindow, Tinput, Tcontext>::is_key_up(Keys key) const
    { return input->is_key_up(key); }

    template<class Twindow, class Tinput, class Tcontext>
    inline i16 PlatformManager<Twindow, Tinput, Tcontext>::get_xmouse() const
    { return input->get_xmouse(); }

    template<class Twindow, class Tinput, class Tcontext>
    inline i16 PlatformManager<Twindow, Tinput, Tcontext>::get_ymouse() const
    { return input->get_ymouse(); }

    template<class Twindow, class Tinput, class Tcontext>
    inline i16 PlatformManager<Twindow, Tinput, Tcontext>::get_mouse_wheel() const
    { return input->get_mouse_wheel(); }

    template<class Twindow, class Tinput, class Tcontext>
    inline void PlatformManager<Twindow, Tinput, Tcontext>::close_window()
    { window->close(); }

    template<class Twindow, class Tinput, class Tcontext>
    inline std::unique_ptr<Twindow>& PlatformManager<Twindow, Tinput, Tcontext>::get_window()
	{ return window; }
}

#endif // JOJ_PLATFORM_MANAGER_H
