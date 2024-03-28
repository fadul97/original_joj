#ifndef JOJ_WIN32_GL_CONTEXT_H
#define JOJ_WIN32_GL_CONTEXT_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include "platform/context/context.h"
#include "platform/win32/window_win32.h"

namespace joj
{
    class JAPI Win32GLContext : public GraphicsContext<Win32Window>
    {
    public:
        Win32GLContext();
        ~Win32GLContext();

        b8 create(std::unique_ptr<Win32Window>& window) override;
        void make_current(std::unique_ptr<Win32Window>& window) override;
        void destroy() override;
          
    private:  
        HGLRC m_rc;
        u32 m_color_bits;
        u32 m_depth_bits;
        i32 m_pixel_format_attrib_list[19];
        u32 m_context_attribs[16];

        u32 m_gl_version_major;
        u32 m_gl_version_minor;

        void log_hardware_info() override;
        b8 is_extension_supported(const char *ext_list, const char *extension);
    };
}

#endif // JPLATFORM_WINDOWS

#endif // JOJ_WIN32_GL_CONTEXT_H
