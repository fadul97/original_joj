#ifndef JOJ_X11_TIMER_H
#define JOJ_X11_TIMER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_LINUX

#include "platform/timer.h"

namespace joj
{
    class JAPI X11Timer : public Timer
    {
    public:
        X11Timer();
        ~X11Timer();

        f32 reset() override;
        f32 elapsed() override;
        b8 was_elapsed(f32 secs) override;
    };

    // TODO: Implement reset()
    inline f32 X11Timer::reset()
    { return 1.0f; }
    
    // TODO: Implement was_elapsed(f32 secs)
    inline b8 X11Timer::was_elapsed(f32 secs)
    { return false; }
}

#endif // JPLATFORM_LINUX

#endif // JOJ_X11_TIMER_H
