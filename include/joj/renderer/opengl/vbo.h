#ifndef JOJ_GL_VBO_H
#define JOJ_GL_VBO_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_LINUX

#include "resources/geometry/vertex.h"
#include "graphics/x11/joj_gl_x11.h"

namespace joj
{
    class JAPI VBO
    {
    public:
        VBO();
        ~VBO();

        void bind() const;
        void unbind() const;

        void delete_vbo();

        void bind_buffer_data(GLsizeiptr size, const Vertex* data);

        void specify_position_data(GLsizei stride, const GLvoid * pointer, u32 location = 0);
        void specify_color_data(GLsizei stride, const GLvoid * pointer, u32 location = 1);

        u32 id;
        u32 position_location;
        u32 color_location;
    };
}

#endif // JPLATFORM_LINUX

#endif // JOJ_GL_VBO_H
