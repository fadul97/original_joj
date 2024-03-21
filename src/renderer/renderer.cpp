#include "renderer/renderer.h"

#if JPLATFORM_LINUX
#include "platform/x11/window_x11.h"
#elif JPLATFORM_WINDOWS
#include "platform/win32/window_win32.h"
#endif

template<class Twindow>
joj::JIRenderer<Twindow>::JIRenderer()
{
}

template<class Twindow>
joj::JIRenderer<Twindow>::~JIRenderer()
{
}

#if JPLATFORM_LINUX
template class joj::JIRenderer<joj::X11Window>;
#elif JPLATFORM_WINDOWS
template class joj::JIRenderer<joj::Win32Window>;
#endif
