#ifndef JOJ_GL_RENDERER_H
#define JOJ_GL_RENDERER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "renderer/renderer.h"

#define JOJ_GL_DEFINE_EXTERN
#include "platform/context/opengl/joj_gl.h"

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
    class JAPI GLRenderer : public IRenderer<JojWindow>
    {
    public:
        GLRenderer();
        ~GLRenderer();
        
        b8 init(std::unique_ptr<JojWindow>& window) override;
        void render() override;
        void clear() override;
        void clear(f32 r, f32 g, f32 b, f32 a) override;
        void swap_buffers() override;
        void shutdown() override;

        void set_polygon_mode(GLenum face, GLenum mode) const;

        ErrorCode setup_default_pipeline(std::unique_ptr<JojWindow>& window) override;

    private:
        f32 bg_color[4];
    };

    inline void GLRenderer::set_polygon_mode(GLenum face, GLenum mode) const
    { glPolygonMode(face, mode); }
}



#endif // JOJ_GL_RENDERER_H
