#ifndef JOJ_GL_VAO_H
#define JOJ_GL_VAO_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_LINUX

#include "vbo.h"
#include "ebo.h"
#include "resources/geometry/vertex.h"
#include "resources/geometry/geometry.h"
#include "graphics/x11/joj_gl_x11.h"

namespace joj
{
    class JAPI VAO
    {
    public:
        VAO();
        ~VAO();

        void bind() const;
        void unbind() const;

        void bind_buffer_data(GLsizeiptr vertex_size, const Vertex* vertex_data, GLsizeiptr index_size, const u32* index_data);
        void bind_buffer_data(Geometry& geometry);

        u32 id;
        VBO vbo;
        EBO ebo;
    };
}

#endif // JPLATFORM_LINUX

#endif // JOJ_GL_VAO_H
