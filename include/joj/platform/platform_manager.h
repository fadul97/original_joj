#ifndef JOJ_PLATFORM_MANAGER_H
#define JOJ_PLATFORM_MANAGER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <string>
#include "window.h"
#include "input.h"
#include "graphics/context.h"
#include <memory>

namespace joj
{
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

        void close_window();

        virtual b8 init(i16 width = 800, i16 height = 600, std::string title = std::string{ "Joj PlatformManager" }) = 0;
        virtual b8 create_window() = 0;
        virtual b8 create_simple_window(i16 width = 800, i16 height = 600, std::string title = std::string{ "Joj PlatformManager" }) = 0;
        virtual void process_events() = 0;
        virtual void swap_buffers() = 0;
        virtual void shutdown() = 0;

        virtual std::unique_ptr<Window>& get_window();

    protected:  
        std::unique_ptr<Window> window;
        std::unique_ptr<Input> input;
        std::unique_ptr<GraphicsContext> context;
    };

    inline b8 PlatformManager::is_running() const
    { return window->is_running(); }

    inline b8 PlatformManager::is_key_down(Keys key) const
    { return input->is_key_down(key); }

    inline b8 PlatformManager::is_key_pressed(Keys key) const
    { return input->is_key_pressed(key); }

    inline b8 PlatformManager::is_key_up(Keys key) const
    { return input->is_key_up(key); }

    inline i16 PlatformManager::get_xmouse() const
    { return input->get_xmouse(); }

    inline i16 PlatformManager::get_ymouse() const
    { return input->get_ymouse(); }

    inline i16 PlatformManager::get_mouse_wheel() const
    { return input->get_mouse_wheel(); }

    inline void PlatformManager::close_window()
    { window->close(); }

    inline std::unique_ptr<Window>& PlatformManager::get_window()
	{ return window; }
}

#endif // JOJ_PLATFORM_MANAGER_H
