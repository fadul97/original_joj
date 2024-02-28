#ifndef JOJ_VIEWPORT_H
#define JOJ_VIEWPORT_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

namespace joj
{
    class JAPI Viewport
    {
    public:
        Viewport();
        Viewport(f32 xtop_left, f32 ytop_left, u16 width, u16 height, f32 min_depth, f32 max_depth);
        ~Viewport();

    private:
        f32 m_xtop_left;
        f32 m_ytop_left;
        u16 m_width;
        u16 m_height;
        f32 m_min_depth;
        f32 m_max_depth;
    };
}

#endif // JOJ_VIEWPORT_H
