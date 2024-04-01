#include "platform/platform_manager.h"

#if JPLATFORM_LINUX
#include "platform/x11/window_x11.h"
#include "platform/x11/input_x11.h"
#elif JPLATFORM_WINDOWS
#include "platform/win32/window_win32.h"
#include "platform/win32/input_win32.h"
#endif // JPLATFORM_WINDOWS

template<class Twindow, class Tinput>
joj::PlatformManager<Twindow, Tinput>::PlatformManager()
{
}

template<class Twindow, class Tinput>
joj::PlatformManager<Twindow, Tinput>::~PlatformManager()
{
}

#if JPLATFORM_LINUX
template class joj::PlatformManager<joj::X11Window, joj::X11Input>;
#elif JPLATFORM_WINDOWS
template class joj::PlatformManager<joj::Win32Window, joj::Win32Input>;
#endif // JPLATFORM_WINDOWS
