#ifndef JOJ_ICON_IMAGE_H
#define JOJ_ICON_IMAGE_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

namespace joj
{
    struct IconImage
    {
        i32 width;
        i32 height;
        u8* pixels;
    };
}

#endif // JOJ_ICON_IMAGE_H
