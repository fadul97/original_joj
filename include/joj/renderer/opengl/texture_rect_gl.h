#ifndef JOJ_GL_TEXTURE_RECT_H
#define JOJ_GL_TEXTURE_RECT_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "math/vector4.h"
#include "math/vector2.h"

namespace joj
{
    class JAPI GLTextureRect
    {
    public:
        GLTextureRect();
        GLTextureRect(f32 x, f32 y, f32 sprite_width, f32 sprite_height, f32 sheet_width, f32 sheet_height);
        ~GLTextureRect();

        f32 m_x0;
        f32 m_y0;
        f32 m_x1;
        f32 m_y1;
    };
}

#endif // JOJ_GL_TEXTURE_RECT_H
