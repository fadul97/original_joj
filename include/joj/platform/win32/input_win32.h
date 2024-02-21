#ifndef JOJ_WIN32_INPUT_H
#define JOJ_WIN32_INPUT_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include "platform/input.h"
#include "platform/win32/window_win32.h"

namespace joj
{
    // ATTENTION: input must be initialized after window creation
    class JAPI Win32Input : public Input
    {
    public:
        Win32Input();
        ~Win32Input();

        static LRESULT CALLBACK InputProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    };
}

#endif // JPLATFORM_WINDOWS

#endif // JOJ_WIN32_INPUT_H
