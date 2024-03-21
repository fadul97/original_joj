#ifndef JOJ_RENDERER_H
#define JOJ_RENDERER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <memory>
#include "platform/window.h"

namespace joj
{
    template<class Twindow>
    class JAPI JIRenderer
    {
    public:
        JIRenderer();
        virtual ~JIRenderer() = 0;

        virtual b8 init(std::unique_ptr<Twindow>& window) = 0;
        virtual void render() = 0;
        virtual void clear() = 0;
        virtual void clear(f32 r, f32 g, f32 b, f32 a) = 0;
        virtual void swap_buffers() = 0;
        virtual void shutdown() = 0;
    };
}

#endif // JOJ_RENDERER_H
