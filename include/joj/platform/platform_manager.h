#ifndef JOJ_PLATFORM_MANAGER_H
#define JOJ_PLATFORM_MANAGER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <string>
#include "window.h"
#include <memory>

namespace joj
{
    class JAPI PlatformManager
    {
    public:
        PlatformManager();
        virtual ~PlatformManager();

        b8 is_running() const;

        void close_window();

        virtual b8 init(i16 width = 800, i16 height = 600, std::string title = std::string{ "Joj PlatformManager" }) = 0;
        virtual b8 create_window() = 0;
        virtual b8 create_simple_window(i16 width = 800, i16 height = 600, std::string title = std::string{ "Joj PlatformManager" }) = 0;
        virtual void process_events() = 0;
        virtual void swap_buffers() = 0;
        virtual void shutdown() = 0;

    protected:  
        std::unique_ptr<Window> window;
    };

    inline b8 PlatformManager::is_running() const
    { return window->is_running(); }

    inline void PlatformManager::close_window()
    { window->close(); }
}

#endif // JOJ_PLATFORM_MANAGER_H
