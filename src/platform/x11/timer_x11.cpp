#include "platform/x11/timer_x11.h"
#include <ctime>

#if JPLATFORM_LINUX

joj::X11Timer::X11Timer()
{
    stopped = false;
}

joj::X11Timer::~X11Timer()
{
}

f32 joj::X11Timer::elapsed()
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC_RAW, &now);
    return now.tv_sec + now.tv_nsec * 0.000000001;
}

#endif // JPLATFORM_LINUX
