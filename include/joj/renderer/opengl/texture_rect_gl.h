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
        GLTextureRect(f32 x0, f32 y0, f32 x1, f32 y1, f32 sprite_width, f32 sprite_height);
        ~GLTextureRect();

        // union
        // {
            Vector4 rect;
        //     struct
        //     {
        //         Vector2 bottom_left;
        //         Vector2 top_right;
        //     };
        // };
    };
}

#endif // JOJ_GL_TEXTURE_RECT_H
