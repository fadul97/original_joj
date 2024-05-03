#include "platform/window.h"

#if JPLATFORM_LINUX
#include "platform/x11/window_x11.h"
#elif JPLATFORM_WINDOWS
#include "platform/win32/window_win32.h"
#endif

template<typename TWindowConfig>
joj::Window<TWindowConfig>::Window(const i16 width, const i16 height, const char* title, const WindowMode mode)
    : m_width(width), m_height(height), m_title(title), m_running(false), m_mode(mode)
{
}

template<typename TWindowConfig>
joj::Window<TWindowConfig>::~Window() = default;

#if JPLATFORM_LINUX
template class joj::Window<u32>;
#elif JPLATFORM_WINDOWS
template class joj::Window<HWND>;
#endif
