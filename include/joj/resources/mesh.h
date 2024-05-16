#ifndef JOJ_MESH_H
#define JOJ_MESH_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <vector>
#include "vertex.h"

namespace joj
{
    class JAPI Mesh
    {
    public:
        Mesh();
        ~Mesh();

        std::vector<Vertex> vertices;
        std::vector<u32> indices;

        void translate(f32 dx, f32 dy, f32 dz);
        void move_to(f32 px, f32 py, f32 pz);

        [[nodiscard]] Vector3 get_position() const;

        [[nodiscard]] const Vertex* get_vertex_data() const;
        [[nodiscard]] const u32* get_index_data() const;

        [[nodiscard]] u32 get_vertex_count() const;
        [[nodiscard]] u32 get_index_count() const;

    protected:
        Vector3 m_position{};
    };

    inline void Mesh::translate(const f32 dx, const f32 dy, const f32 dz)
    { m_position.x += dx; m_position.y += dy; m_position.z += dz; }

    inline void Mesh::move_to(const f32 px, const f32 py, const f32 pz)
    { m_position.x = px; m_position.y = py; m_position.z = pz; }

    inline Vector3 Mesh::get_position() const
    { return m_position; }

    inline const Vertex* Mesh::get_vertex_data() const
    { return vertices.data(); }

    inline const u32* Mesh::get_index_data() const
    { return indices.data(); }

    inline u32 Mesh::get_vertex_count() const
    { return static_cast<u32>(vertices.size()); }

    inline u32 Mesh::get_index_count() const
    { return static_cast<u32>(indices.size()); }
}

#endif // JOJ_MESH_H
