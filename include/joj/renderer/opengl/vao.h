#ifndef JOJ_GL_VAO_H
#define JOJ_GL_VAO_H

#include <vector>
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

        u32 create_vbo();
        u32 create_ebo();

        void bind_vbo(u32 binding_index, u32 vbo_id, GLintptr offset, GLintptr stride);
        void unbind_vbo();

        void bind_ebo(u32 ebo_id);
        
        void specify_position_data(u32 bindind_index, i32 size, u32 offset, u32 attrib_location = 0);
        void specify_color_data(u32 bindind_index, i32 size, u32 offset, u32 attrib_location = 1);
        void specify_texture_data(u32 bindind_index, i32 size, u32 offset, u32 attrib_location = 2);

        void bind_vbo_data(u32 vbo_id, GLsizeiptr vertex_size, const void* vertex_data);
        void bind_ebo_data(u32 ebo_id, GLsizeiptr index_size, const void* index_data);

        void bind_buffer_data(GLsizeiptr vertex_size, const Vertex* vertex_data, GLsizeiptr index_size, const u32* index_data);
        void bind_buffer_data(Geometry& geometry);

        u32 id;
        VBO vbo;
        EBO ebo;

        std::vector<VBO> vbos;
        std::vector<EBO> ebos;

        u32 position_location;
        u32 color_location;
        u32 texture_location;
    };
}

#endif // JPLATFORM_LINUX

#endif // JOJ_GL_VAO_H
