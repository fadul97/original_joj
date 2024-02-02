#include "renderer/opengl/vao.h"

joj::VAO::VAO()
{
    glGenVertexArrays(1, &id);
    vbo = joj::VBO{};
    ebo = joj::EBO{};
}

joj::VAO::~VAO()
{
}

void joj::VAO::bind() const
{
    glBindVertexArray(id);
}

void joj::VAO::unbind() const
{
    glBindVertexArray(0);
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
