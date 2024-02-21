#ifndef JOJ_GL_RENDERER_H
#define JOJ_GL_RENDERER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_LINUX

#include "renderer/renderer.h"
#include "platform/x11/window_x11.h"
#include "graphics/x11/joj_gl_x11.h"

namespace joj
{
    class JAPI GLRenderer : public Renderer<X11Window>
    {
    public:
        GLRenderer();
        ~GLRenderer();
        
        b8 init(std::unique_ptr<X11Window>& window) override;
        void render() override;
        void clear(f32 r = 0.0f, f32 g = 0.0f, f32 b = 0.0f, f32 a = 0.0f) override;
        void shutdown() override;

        void set_polygon_mode(GLenum face, GLenum mode) const;
    };

    inline void GLRenderer::set_polygon_mode(GLenum face, GLenum mode) const
    { glPolygonMode(face, mode); }
}


#endif // JPLATFORM_LINUX

#endif // JOJ_GL_RENDERER_H
