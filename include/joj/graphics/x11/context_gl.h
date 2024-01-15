#ifndef JOJ_X11_GL_CONTEXT_H
#define JOJ_X11_GL_CONTEXT_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_LINUX

#include "graphics/context.h"

#include <GL/glx.h>

namespace joj
{
    class JAPI X11GLContext : public GraphicsContext
    {
    public:
        X11GLContext();
        ~X11GLContext();

        b8 create(std::unique_ptr<Window>& window) override;
        void make_current(std::unique_ptr<Window>& window) override;
        void destroy() override;
          
    private:  
        GLXContext context;
        GLXFBConfig fb_config;

        void log_hardware_info() override;
        b8 is_extension_supported(const char *ext_list, const char *extension);
    };
}

#endif // JPLATFORM_LINUX

#endif // JOJ_X11_GL_CONTEXT_H
