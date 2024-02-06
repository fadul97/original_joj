#ifndef JOJ_GL_TEXTURE_H
#define JOJ_GL_TEXTURE_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_LINUX
#include "graphics/x11/joj_gl_x11.h"
#endif // JPLATFORM_LINUX

namespace joj
{
    class JAPI GLTexture
    {
    public:
        GLTexture();
        GLTexture(const char* image_path);
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
