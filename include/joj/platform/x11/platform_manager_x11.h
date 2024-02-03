#ifndef JOJ_X11_PLATFORM_MANAGER_H
#define JOJ_X11_PLATFORM_MANAGER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_LINUX

#include "platform/platform_manager.h"

namespace joj
{
    class JAPI X11PlatformManager : public PlatformManager
    {
    public:
        X11PlatformManager();
        ~X11PlatformManager();

        b8 init(i16 width = 800, i16 height = 600, std::string title = std::string{ "Joj X11PlatformManager" }) override;
        b8 create_window() override;
        b8 create_simple_window(i16 width = 800, i16 height = 600, std::string title = std::string{ "Joj X11PlatformManager" }) override;
        void process_events() override;
        void swap_buffers() override;
        void shutdown() override;

        std::unique_ptr<Window>& get_window() override;
    
    private:
        unsigned long delete_msg;
    };

    inline std::unique_ptr<Window>& X11PlatformManager::get_window()
	{ return window; }
}

#endif // JOJ_X11_PLATFORM_MANAGER_H

#endif // JPLATFORM_LINUX
