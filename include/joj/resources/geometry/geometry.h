#ifndef JOJ_GEOMETRY_H
#define JOJ_GEOMETRY_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "resources/vertex.h"
#include <vector>

namespace joj
{
    enum class GeometryType { UNKNOWN, CUBE, CYLINDER, SPHERE, GEOSPHERE, GRID, QUAD };

    class JAPI Geometry
    {
    public:
        Geometry();
        virtual ~Geometry();

        std::vector<Vertex> vertices;
        std::vector<u32> indices;

        virtual f32 x() const;
        virtual f32 y() const;
        virtual f32 z() const;

        virtual void translate(f32 dx, f32 dy, f32 dz);
		virtual void move_to(f32 px, f32 py, f32 pz);

        virtual Vector3 get_position() const;
        virtual GeometryType get_type() const;

        const Vertex* get_vertex_data() const;
        const u32* get_index_data() const;

        u32 get_vertex_count() const;
        u32 get_index_count() const;

    protected:
        Vector3 position;
        GeometryType type;

        void subdivide();
    };

    
    inline f32 Geometry::x() const
    { return position.x; }
    
    inline f32 Geometry::y() const
    { return position.y; }
    
    inline f32 Geometry::z() const
    { return position.z; }
    
    inline void Geometry::translate(f32 dx, f32 dy, f32 dz)
    { position.x += dx; position.y += dy; position.z += dz; }

    inline void Geometry::move_to(f32 px, f32 py, f32 pz)
    { position.x = px; position.y = py; position.z = pz; }

    inline Vector3 Geometry::get_position() const
    { return position; }
    
    inline GeometryType Geometry::get_type() const
    { return type; }
    
    inline const Vertex* Geometry::get_vertex_data() const
    { return vertices.data(); }
    
    inline const u32* Geometry::get_index_data() const
    { return indices.data(); }
    
    inline u32 Geometry::get_vertex_count() const
    { return static_cast<u32>(vertices.size()); }
    
    inline u32 Geometry::get_index_count() const
    { return static_cast<u32>(indices.size()); }
}

#endif // JOJ_GEOMETRY_H
