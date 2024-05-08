#ifndef JOJ_TIMER_H
#define JOJ_TIMER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

namespace joj
{
    class JAPI Timer
    {
    public:
        Timer();
        virtual ~Timer();

        virtual void start() = 0;
        virtual void stop() = 0;
        virtual f32 reset() = 0;
        virtual f32 elapsed() = 0;
        virtual b8 was_elapsed(f32 secs) = 0;
        virtual void begin_period() = 0;
        virtual void end_period() = 0;
          
    protected:
        b8 m_stopped;
    };
}

#endif // JOJ_TIMER_H
