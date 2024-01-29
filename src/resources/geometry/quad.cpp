#include "resources/geometry/quad.h"

joj::Quad::Quad()
{
    type = GeometryType::QUAD;
}

joj::Quad::Quad(f32 width, f32 height)
{
    type = GeometryType::QUAD;

    f32 w = 0.5f * width;
    f32 h = 0.5f * height;

    // Create vertex buffer
    Vertex quad_vertices[4] =
    {
        { Vector3{-w, -h, 0.0f}, Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { Vector3{-w, +h, 0.0f}, Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { Vector3{+w, +h, 0.0f}, Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { Vector3{+w, -h, 0.0f}, Vector4{1.0f, 1.0f, 0.0f, 1.0f} }
    };

    // Add vertices to mesh
    for (const Vertex& v : quad_vertices)
        vertices.push_back(v);

    // Create index buffer
    u32 quad_indices[6] =
    {
        0, 1, 2,
        0, 2, 3
    };

    // insere índices na malha
    for (u32 i : quad_indices)
        indices.push_back(i);
    
}

joj::Quad::Quad(f32 width, f32 height, Vector4 color)
{
    

    type = GeometryType::QUAD;

    f32 w = 0.5f * width;
    f32 h = 0.5f * height;

    // Create vertex buffer
    Vertex quad_vertices[4] =
    {
        { Vector3{-w, -h, 0.0f}, color },
        { Vector3{-w, +h, 0.0f}, color },
        { Vector3{+w, +h, 0.0f}, color },
        { Vector3{+w, -h, 0.0f}, color }
    };

    // Add vertices to mesh
    for (const Vertex& v : quad_vertices)
        vertices.push_back(v);

    // Create index buffer
    u32 quad_indices[6] =
    {
        0, 1, 2,
        0, 2, 3
    };

    // insere índices na malha
    for (u32 i : quad_indices)
        indices.push_back(i);
    
}
