#ifndef JOJ_WIN32_TIMER_H
#define JOJ_WIN32_TIMER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include "platform/timer.h"
#include <windows.h>

namespace joj
{
    class JAPI Win32Timer : public Timer
    {
    public:
        Win32Timer();
        ~Win32Timer() override;

        void start() override;
        void stop() override;
        f32 reset() override;
        f32 elapsed() override;
        b8 was_elapsed(f32 secs) override;
        void begin_period() override;
        void end_period() override;

    private:
        LARGE_INTEGER m_counter_start;
        LARGE_INTEGER m_end;
        LARGE_INTEGER m_freq;
    };

    inline b8 Win32Timer::was_elapsed(const f32 secs)
    { return (elapsed() >= secs ? true : false); }
    
    inline void Win32Timer::begin_period()
    { timeBeginPeriod(1); }

    inline void Win32Timer::end_period()
    { timeEndPeriod(1); }
}

#endif // JPLATFORM_WINDOWS

#endif // JOJ_WIN32_TIMER_H
