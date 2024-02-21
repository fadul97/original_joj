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

        void start(){}
        void stop(){}
        virtual f32 reset() = 0;
        virtual f32 elapsed() = 0;
        virtual b8 was_elapsed(f32 secs) = 0;
        void time_begin_period(){}
        void time_end_period(){}
          
    protected:
        b8 m_stopped;
    };
}

#endif // JOJ_TIMER_H
