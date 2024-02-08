#ifndef JOJ_GL_EBO_H
#define JOJ_GL_EBO_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_LINUX

#include "graphics/x11/joj_gl_x11.h"

namespace joj
{
    class JAPI EBO
    {
    public:
        EBO();
        ~EBO();

        void bind() const;

        void delete_ebo();

        void bind_buffer_data(GLsizeiptr size, const u32* data);

        u32 id;
    };
}

#endif // JPLATFORM_LINUX

#endif // JOJ_GL_EBO_H
