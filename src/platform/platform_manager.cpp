#include "platform/platform_manager.h"

#if JPLATFORM_LINUX
#include "graphics/opengl/x11/context_gl.h"
#include "platform/x11/input_x11.h"
#include "platform/x11/window_x11.h"
#elif JPLATFORM_WINDOWS
#include "graphics/opengl/win32/context_gl.h"
#include "platform/win32/input_win32.h"
#include "platform/win32/window_win32.h"
#endif // JPLATFORM_WINDOWS

template<class Twindow, class Tinput, class Tcontext>
joj::PlatformManager<Twindow, Tinput, Tcontext>::PlatformManager()
{
    window = nullptr;
    input = nullptr;
    context = nullptr;
}

template<class Twindow, class Tinput, class Tcontext>
joj::PlatformManager<Twindow, Tinput, Tcontext>::~PlatformManager()
{
}

#if JPLATFORM_LINUX
template class joj::PlatformManager<joj::X11Window, joj::X11Input, joj::X11GLContext>;
#elif JPLATFORM_WINDOWS
template class joj::PlatformManager<joj::Win32Window, joj::Win32Input, joj::Win32GLContext>;
#endif // JPLATFORM_WINDOWS
