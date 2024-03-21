#include "platform/platform_manager.h"

#if JPLATFORM_LINUX
#include "platform/x11/window_x11.h"
#elif JPLATFORM_WINDOWS
#include "platform/win32/window_win32.h"
#endif // JPLATFORM_WINDOWS

template<class Twindow>
joj::IPlatformManager<Twindow>::IPlatformManager()
{
}

template<class Twindow>
joj::IPlatformManager<Twindow>::~IPlatformManager()
{
}

#if JPLATFORM_LINUX
template class joj::IPlatformManager<joj::X11Window>;
#elif JPLATFORM_WINDOWS
template class joj::IPlatformManager<joj::Win32Window>;
#endif // JPLATFORM_WINDOWS
