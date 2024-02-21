#include "renderer/renderer.h"

#if JPLATFORM_LINUX
#include "platform/x11/window_x11.h"
#endif // JPLATFORM_LINUX

template<class Twindow>
joj::Renderer<Twindow>::Renderer()
{
}

template<class Twindow>
joj::Renderer<Twindow>::~Renderer()
{
}

#if JPLATFORM_LINUX
template class joj::Renderer<joj::X11Window>;
#endif // JPLATFORM_LINUX
