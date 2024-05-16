#ifndef JOJ_GL_RENDERER_H
#define JOJ_GL_RENDERER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "renderer/renderer.h"

#define JOJ_GL_DEFINE_EXTERN
#include "platform/context/opengl/joj_gl.h"

#include "platform/win32/window_win32.h"

namespace joj
{
    class JAPI GLRenderer : public IRenderer<WindowConfig>
    {
    public:
        GLRenderer();
        ~GLRenderer() override;
        
        b8 init(WindowConfig& window) override;
        void render() override;
        void clear() override;
        void clear(f32 r, f32 g, f32 b, f32 a) override;
        void swap_buffers() override;
        void shutdown() override;

        static void set_polygon_mode(GLenum face, GLenum mode);

        ErrorCode setup_default_pipeline(WindowConfig& window) override;

    private:
        f32 bg_color[4];
    };

    inline void GLRenderer::set_polygon_mode(const GLenum face, const GLenum mode)
    { glPolygonMode(face, mode); }
}



#endif // JOJ_GL_RENDERER_H
