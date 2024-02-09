#include "renderer/opengl/texture_rect_gl.h"

joj::GLTextureRect::GLTextureRect()
    : rect{Vector4{}}
{
}

joj::GLTextureRect::GLTextureRect(f32 x0, f32 y0, f32 x1, f32 y1, f32 sprite_width, f32 sprite_height)
    : rect {Vector4{x0 / sprite_width, y0 / sprite_height, x1 / sprite_width, y1 / sprite_height}}
{
}

joj::GLTextureRect::~GLTextureRect()
{
}
