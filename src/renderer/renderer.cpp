#include "renderer/renderer.h"

template<class Twindow>
joj::IRenderer<Twindow>::IRenderer() = default;

template<class Twindow>
joj::IRenderer<Twindow>::~IRenderer() = default;

#if JPLATFORM_LINUX
#include "platform/x11/window_x11.h"
template class joj::IRenderer<joj::X11Window>;
#elif JPLATFORM_WINDOWS
#include "platform/win32/window_win32.h"
template class joj::IRenderer<joj::WindowConfig>;
#endif
