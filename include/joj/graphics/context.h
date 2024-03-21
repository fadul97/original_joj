#ifndef JOJ_GRAPHICS_CONTEXT_H
#define JOJ_GRAPHICS_CONTEXT_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <memory>
#include "platform/window.h"

namespace joj
{
    enum class BackendRenderer { DX11, GL };

    template<class Twindow>
    class JAPI GraphicsContext
    {
    public:
        GraphicsContext();
        virtual ~GraphicsContext();

        virtual b8 create(std::unique_ptr<Twindow>& window) = 0;
        virtual void make_current(std::unique_ptr<Twindow>& window) = 0;
        virtual void destroy() = 0;
          
    protected:
        virtual void log_hardware_info() = 0;
    };
}


#endif // JOJ_GRAPHICS_CONTEXT_H
