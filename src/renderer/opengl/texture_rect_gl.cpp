#include "renderer/opengl/texture_rect_gl.h"

joj::GLTextureRect::GLTextureRect()
    : m_x0{0.0f}, m_y0{0.0f}, m_x1{1.0f}, m_y1{1.0f}
{
}

joj::GLTextureRect::GLTextureRect(f32 x, f32 y, f32 sprite_width, f32 sprite_height, f32 sheet_width, f32 sheet_height)
    : m_x0{(x * sprite_width) / sheet_width},
      m_y0{(y * sprite_height) / sheet_height}, 
      m_x1{((x + 1) * sprite_width) / sheet_width},
      m_y1{((y + 1) * sprite_height) / sheet_height}
{
}

joj::GLTextureRect::~GLTextureRect()
{
}
