#include "graphics/context.h"

#if JPLATFORM_LINUX
#include "platform/x11/window_x11.h"
#endif // JPLATFORM_LINUX

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
#endif // JPLATFORM_LINUX
