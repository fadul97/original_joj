#include "platform/platform_manager.h"

#if JPLATFORM_LINUX
#include "graphics/x11/context_gl.h"
#include "platform/x11/input_x11.h"
#include "platform/x11/window_x11.h"
#endif // JPLATFORM_LINUX

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
#endif // JPLATFORM_LINUX
