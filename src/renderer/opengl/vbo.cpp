#include "renderer/opengl/vbo.h"

joj::VBO::VBO()
{
    glGenBuffers(1, &id);

    position_location = 0;
    color_location = 1;
}

joj::VBO::~VBO()
{
}

void joj::VBO::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void joj::VBO::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void joj::VBO::bind_buffer_data(GLsizeiptr size, const Vertex* data)
{
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void joj::VBO::specify_position_data(GLsizei stride, const GLvoid * pointer, u32 location)
{
    position_location = location;
    glEnableVertexAttribArray(position_location);
    glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, stride, pointer);
}

void joj::VBO::specify_color_data(GLsizei stride, const GLvoid * pointer, u32 location)
{
    color_location = location;
    glEnableVertexAttribArray(color_location);
    glVertexAttribPointer(color_location, 4, GL_FLOAT, GL_FALSE, stride, pointer);
}
