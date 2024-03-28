#include "renderer/opengl/vao.h"

#define JOJ_GL_DEFINE_EXTERN
#include "platform/context/opengl/joj_gl.h"

joj::VAO::VAO()
{
    glGenVertexArrays(1, &id);
    vbo = joj::VBO{};
    ebo = joj::EBO{};

    vbos = std::vector<VBO>{};
    ebos = std::vector<EBO>{};

    position_location = 0;
    color_location = 1;
    texture_location = 2;
}

joj::VAO::~VAO()
{
    glDeleteVertexArrays(1, &id);
}

void joj::VAO::bind() const
{
    glBindVertexArray(id);
}

void joj::VAO::unbind() const
{
    glBindVertexArray(0);
}

void joj::VAO::delete_vao()
{
    glDeleteVertexArrays(1, &id);
}

u32 joj::VAO::create_vbo()
{
    vbos.push_back(VBO{});
    glCreateBuffers(1, &vbos.back().id);
    return vbos.back().id;
}

u32 joj::VAO::create_ebo()
{
    ebos.push_back(EBO{});
    glCreateBuffers(1, &ebos.back().id);
    return ebos.back().id;
}

void joj::VAO::bind_vbo(u32 binding_index, u32 vbo_id, GLintptr offset, GLintptr stride)
{
    glBindVertexBuffer(binding_index, vbo_id, offset, stride);
}

void joj::VAO::unbind_vbo()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void joj::VAO::bind_ebo(u32 ebo_id)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
}

void joj::VAO::specify_position_data(u32 bindind_index, i32 size, u32 offset, u32 attrib_location)
{
    position_location = attrib_location;
    glEnableVertexAttribArray(position_location);
    glVertexAttribFormat(position_location, size, GL_FLOAT, GL_FALSE, offset);
    glVertexAttribBinding(position_location, bindind_index);
}

void joj::VAO::specify_color_data(u32 bindind_index, i32 size, u32 offset, u32 attrib_location)
{
    color_location = attrib_location;
    glEnableVertexAttribArray(color_location);
    glVertexAttribFormat(color_location, size, GL_FLOAT, GL_FALSE, offset);
    glVertexAttribBinding(color_location, bindind_index);
}

void joj::VAO::specify_texture_data(u32 bindind_index, i32 size, u32 offset, u32 attrib_location)
{
    texture_location = attrib_location;
    glEnableVertexAttribArray(texture_location);
    glVertexAttribFormat(texture_location, size, GL_FLOAT, GL_FALSE, offset);
    glVertexAttribBinding(texture_location, bindind_index);
}

void joj::VAO::bind_vbo_data(u32 vbo_id, GLsizeiptr vertex_size, const void* vertex_data)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, vertex_size, vertex_data, GL_STATIC_DRAW);
}

void joj::VAO::bind_ebo_data(u32 ebo_id, GLsizeiptr index_size, const void* index_data)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size, index_data, GL_STATIC_DRAW);
}

void joj::VAO::bind_buffer_data(GLsizeiptr vertex_size, const Vertex* vertex_data, GLsizeiptr index_size, const u32* index_data)
{
    bind();
    
    vbo.bind();
    vbo.bind_buffer_data(vertex_size, vertex_data);

    ebo.bind();
    ebo.bind_buffer_data(index_size, index_data);

    // TODO: Abstract position and color specification
    vbo.specify_position_data(sizeof(Vertex), (void*)0);
    vbo.specify_color_data(sizeof(Vertex), (void*)(3 * sizeof(f32)));

    vbo.unbind();

    unbind();
}

void joj::VAO::bind_buffer_data(Geometry& geometry)
{
    bind();

    vbo.bind();
    vbo.bind_buffer_data(geometry.get_vertex_count() * sizeof(Vertex), geometry.get_vertex_data());

    ebo.bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry.get_index_count() * sizeof(u32), geometry.get_index_data(), GL_STATIC_DRAW);

    vbo.specify_position_data(sizeof(Vertex), (void*)0);
    vbo.specify_color_data(sizeof(Vertex), (void*)(3 * sizeof(f32)));

    vbo.unbind();

    unbind();
}
