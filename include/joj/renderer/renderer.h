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
        virtual void clear() = 0;
        virtual void shutdown() = 0;
    };
}

#endif // JOJ_RENDERER_H
