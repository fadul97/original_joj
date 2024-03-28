#include "platform/context/context.h"

#if JPLATFORM_LINUX
#include "platform/x11/window_x11.h"
#elif JPLATFORM_WINDOWS
#include "platform/win32/window_win32.h"
#endif // JPLATFORM_WINDOWS

template<class Twindow>
joj::GraphicsContext<Twindow>::GraphicsContext()
{
}

template<class Twindow>
joj::GraphicsContext<Twindow>::~GraphicsContext()
{
}

#if JPLATFORM_LINUX
template class joj::GraphicsContext<joj::X11Window>;
#elif JPLATFORM_WINDOWS
template class joj::GraphicsContext<joj::Win32Window>;
#endif // JPLATFORM_WINDOWS
