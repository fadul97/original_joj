#include "renderer/opengl/texture_rect_gl.h"

joj::GLTextureRect::GLTextureRect()
    : m_x0{0.0f}, m_y0{0.0f}, m_x1{1.0f}, m_y1{1.0f}
{
}

joj::GLTextureRect::GLTextureRect(f32 x0, f32 y0, f32 x1, f32 y1, f32 sprite_width, f32 sprite_height)
    : m_x0{x0 / sprite_width}, m_y0{y0 / sprite_height}, m_x1{x1 / sprite_width}, m_y1{y1 / sprite_height}
{
}

joj::GLTextureRect::~GLTextureRect()
{
}
