#ifndef JOJ_GL_ANIMATION_2D_H
#define JOJ_GL_ANIMATION_2D_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#define JOJ_GL_DEFINE_EXTERN
#include "graphics/opengl/joj_gl.h"

#include <vector>
#include "texture_gl.h"
#include "texture_rect_gl.h"

namespace joj
{
    class JAPI GLAnimation2D
    {
    public:
        GLAnimation2D();
        ~GLAnimation2D();

        void add_frame(GLTextureRect rect);

        std::vector<GLTextureRect> m_frames;

        // FIXME: does it need to be a pointer?
        GLTexture* texture;
    };
}

#endif // JOJ_GL_ANIMATION_2D_H
