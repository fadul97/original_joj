#ifndef JOJ_GL_EBO_H
#define JOJ_GL_EBO_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#define JOJ_GL_DEFINE_EXTERN
#include "platform/context/opengl/joj_gl.h"

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

#endif // JOJ_GL_EBO_H
