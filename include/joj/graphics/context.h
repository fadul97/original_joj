#ifndef JOJ_GRAPHICS_CONTEXT_H
#define JOJ_GRAPHICS_CONTEXT_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <memory>
#include "platform/window.h"

namespace joj
{
    class JAPI GraphicsContext
    {
    public:
        GraphicsContext();
        virtual ~GraphicsContext();

        virtual b8 init(std::unique_ptr<Window>& window) = 0;
        virtual void make_current();
          
    protected:
        virtual void log_hardware_info() = 0;
    };
}


#endif // JOJ_GRAPHICS_CONTEXT_H
