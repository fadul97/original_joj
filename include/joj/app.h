#ifndef JOJ_APP_H
#define JOJ_APP_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "engine.h"

namespace joj
{
    class JAPI App
    {
    public:
        App();
        virtual ~App();

        virtual void init() = 0;
        virtual void update(f32 dt) = 0;
        virtual void shutdown() = 0;

        /* These methods have an empty implementation by default
         * and only one of them must be overridden in the derived class:
         * - Draw is executed every cycle of the main loop.
         * - Display is called only once at the start of the application
         * and must be called manually in Update every time the screen needs
         * to be redrawn.
         */
        virtual void draw() {}
        virtual void display() {}
        virtual void on_pause() {}
    
    protected:
        static JPlatformManager* platform_manager;
    };
}

#endif // JOJ_APP_H
