#include "platform/window.h"

#if JPLATFORM_LINUX
#include "platform/x11/window_x11.h"
#endif

template<typename TWindowConfig>
joj::Window<TWindowConfig>::Window()
    : m_title("Abstract Window"), m_running(false), m_window_config()
{
}

template<typename TWindowConfig>
joj::Window<TWindowConfig>::~Window() = default;

#if JPLATFORM_LINUX
template class joj::Window<u32>;
#elif JPLATFORM_WINDOWS
#include "platform/win32/window_win32.h"
template class joj::Window<joj::WindowConfig>;
#endif
