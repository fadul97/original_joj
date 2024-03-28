#ifndef JOJ_GL_TEXTURE_H
#define JOJ_GL_TEXTURE_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#define JOJ_GL_DEFINE_EXTERN
#include "platform/context/opengl/joj_gl.h"

namespace joj
{
    class JAPI GLTexture
    {
    public:
        GLTexture();
        GLTexture(const char* image_path, i32 internal_format, GLenum image_format);
        ~GLTexture();

        void bind() const;
        void create(i32 width, i32 height, u8* data, i32 internal_format, GLenum image_format);

        u32 id;
        i32 width;
        i32 height;

        i32 internal_format;
        GLenum image_format;

        u32 wrap_s;
        u32 wrap_t;
        u32 filter_min;
        u32 filter_max;
    };
}

#endif // JOJ_GL_TEXTURE_H
