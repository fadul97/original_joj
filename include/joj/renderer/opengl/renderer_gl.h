#ifndef JOJ_GL_RENDERER_H
#define JOJ_GL_RENDERER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "renderer/renderer.h"

#define JOJ_GL_DEFINE_EXTERN
#include "graphics/opengl/joj_gl.h"

#if JPLATFORM_LINUX
#include "platform/x11/window_x11.h"
#elif JPLATFORM_WINDOWS
#include "platform/win32/window_win32.h"
#endif // JPLATFORM_WINDOWS

namespace joj
{
#if JPLATFORM_LINUX
    using JojWindow = X11Window;
#elif JPLATFORM_WINDOWS
    using JojWindow = Win32Window;
#endif // JPLATFORM_WINDOWS
}

namespace joj
{
    class JAPI GLRenderer : public Renderer<JojWindow>
    {
    public:
        GLRenderer();
        ~GLRenderer();
        
        b8 init(std::unique_ptr<JojWindow>& window) override;
        void render() override;
        void clear(f32 r = 0.0f, f32 g = 0.0f, f32 b = 0.0f, f32 a = 0.0f) override;
        void shutdown() override;

        void set_polygon_mode(GLenum face, GLenum mode) const;
    };

    inline void GLRenderer::set_polygon_mode(GLenum face, GLenum mode) const
    { glPolygonMode(face, mode); }
}



#endif // JOJ_GL_RENDERER_H
