#ifndef JOJ_RENDERER_H
#define JOJ_RENDERER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <memory>
#include "platform/window.h"

namespace joj
{
    class JAPI Renderer
    {
    public:
        Renderer();
        virtual ~Renderer() = 0;

        virtual b8 init(std::unique_ptr<Window>& window) = 0;
        virtual void render() = 0;
        virtual void clear(f32 r = 0.0f, f32 g = 0.0f, f32 b = 0.0f, f32 a = 0.0f) = 0;
        virtual void shutdown() = 0;
    };
}

#endif // JOJ_RENDERER_H
