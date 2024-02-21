#include "platform/window.h"

#if JPLATFORM_LINUX
#include "platform/x11/window_x11.h"
#endif // JPLATFORM_LINUX

template<typename Tid>
joj::Window<Tid>::Window(i16 width, i16 height, std::string title)
    : width(width), height(height), title(title), running(false)
{
}

template<typename Tid>
joj::Window<Tid>::~Window()
{
}

#if JPLATFORM_LINUX
template class joj::Window<u32>;
#endif // JPLATFORM_LINUX
