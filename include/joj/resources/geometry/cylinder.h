#ifndef JOJ_CYLINDER_H
#define JOJ_CYLINDER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "geometry.h"

namespace joj
{
    class JAPI Cylinder : public Geometry
    {
    public:
        Cylinder();  
        Cylinder(f32 bottom_radius, f32 top_radius, f32 height, u32 slide_count, u32 layer_count);
        Cylinder(f32 bottom_radius, f32 top_radius, f32 height, u32 slide_count, u32 layer_count, Vector4 color);
    };
}

#endif // JOJ_CYLINDER_H
