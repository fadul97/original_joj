#include "platform/context/context.h"

#if JPLATFORM_LINUX
#include "platform/x11/window_x11.h"
#endif // JPLATFORM_LINUX

template<class Twindow>
joj::GraphicsContext<Twindow>::GraphicsContext() = default;

template<class Twindow>
joj::GraphicsContext<Twindow>::~GraphicsContext() = default;

#if JPLATFORM_LINUX
template class joj::GraphicsContext<joj::X11Window>;
#elif JPLATFORM_WINDOWS
// #include "platform/win32/window_win32.h"
// template class joj::GraphicsContext<joj::WindowConfig>;
#endif // JPLATFORM_WINDOWS
