#ifndef JOJ_GRAPHICS_CONTEXT_H
#define JOJ_GRAPHICS_CONTEXT_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "error_code.h"

namespace joj
{
    template<class Twindow>
    class JAPI GraphicsContext
    {
    public:
        GraphicsContext();
        virtual ~GraphicsContext();

        // TODO: Change return value of create and make_current methods
        virtual ErrorCode  create(Twindow& window) = 0;
        virtual ErrorCode  make_current(Twindow& window) = 0;
        virtual void destroy() = 0;
          
    protected:
        virtual void log_hardware_info() = 0;
    };
}

#endif // JOJ_GRAPHICS_CONTEXT_H
