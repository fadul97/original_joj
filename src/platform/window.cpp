#include "platform/window.h"

#if JPLATFORM_LINUX
#include "platform/x11/window_x11.h"
#elif JPLATFORM_WINDOWS
#include "platform/win32/window_win32.h"
#endif

template<typename Tid>
joj::Window<Tid>::Window(i16 width, i16 height, std::string title)
    : width(width), height(height), title(title), running(false), m_mode(WindowMode::WINDOWED)
{
}

template<typename Tid>
joj::Window<Tid>::~Window()
{
}

#if JPLATFORM_LINUX
template class joj::Window<u32>;
#elif JPLATFORM_WINDOWS
template class joj::Window<HWND>;
#endif
