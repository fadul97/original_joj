#ifndef JOJ_X11_INPUT_H
#define JOJ_X11_INPUT_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_LINUX

#include "platform/input.h"

namespace joj
{
    class JAPI X11Input : public Input
    {
    public:
        X11Input();
        ~X11Input();
    };
}

#endif // JPLATFORM_LINUX

#endif // JOJ_X11_INPUT_H
