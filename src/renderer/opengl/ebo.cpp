#include "renderer/opengl/ebo.h"

joj::EBO::EBO()
{
    glGenBuffers(1, &id);
}

joj::EBO::~EBO()
{
    glDeleteBuffers(1, &id);
}

void joj::EBO::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void joj::EBO::delete_ebo()
{
    glDeleteBuffers(1, &id);
}

void joj::EBO::bind_buffer_data(GLsizeiptr size, const u32* data)
{
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

