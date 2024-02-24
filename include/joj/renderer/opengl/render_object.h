#ifndef JOJ_GL_RENDER_OBJECT_H
#define JOJ_GL_RENDER_OBJECT_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "resources/geometry/geometry.h"

namespace joj
{
    // FIXME: Find a better name for class
    class JAPI RenderObject
    {
    public:
        RenderObject(GeometryType type = GeometryType::CUBE);
        ~RenderObject();

        Geometry geometry;
        u32 vbo_id;
        u32 ebo_id;
    };
}

#endif // JOJ_GL_RENDER_OBJECT_H
