#include "resources/geometry/cube.h"

joj::Cube::Cube()
{
    type = GeometryType::CUBE;
}

joj::Cube::Cube(f32 width, f32 height, f32 depth)
{
    type = GeometryType::CUBE;

    f32 w = 0.5f * width;
    f32 h = 0.5f * height;
    f32 d = 0.5f * depth;    

    // Create geometry vertices
    Vertex cube_vertices[8] =
    {
        { Vector3{-w, -h, -d}, Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { Vector3{-w, +h, -d}, Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { Vector3{+w, +h, -d}, Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { Vector3{+w, -h, -d}, Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { Vector3{-w, -h, +d}, Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { Vector3{-w, +h, +d}, Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { Vector3{+w, +h, +d}, Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { Vector3{+w, -h, +d}, Vector4{1.0f, 1.0f, 0.0f, 1.0f} }
    };

    // Add vertices to mesh
    for (const Vertex& v : cube_vertices)
        vertices.push_back(v);

    // Indicate how vertices are interconnected
    u32 cube_indices[36] =
    {
        // front face
        0, 1, 2,
        0, 2, 3,

        // back face
        4, 7, 5,
        7, 6, 5,

        // left face
        4, 5, 1,
        4, 1, 0,

        // right face
        3, 2, 6,
        3, 6, 7,

        // top face
        1, 5, 6,
        1, 6, 2,

        // bottom face
        4, 0, 3,
        4, 3, 7
    };

    // Add indices to mesh
    for (u16 i : cube_indices)
        indices.push_back(i);
}

joj::Cube::Cube(f32 width, f32 height, f32 depth, const Vector4 color)
{
    type = GeometryType::CUBE;

    f32 w = 0.5f * width;
    f32 h = 0.5f * height;
    f32 d = 0.5f * depth;

    // Create geometry vertices
    Vertex cube_vertices[8] =
    {
        { Vector3{-w, -h, -d}, color },
        { Vector3{-w, +h, -d}, color },
        { Vector3{+w, +h, -d}, color },
        { Vector3{+w, -h, -d}, color },
        { Vector3{-w, -h, +d}, color },
        { Vector3{-w, +h, +d}, color },
        { Vector3{+w, +h, +d}, color },
        { Vector3{+w, -h, +d}, color }
    };

    // Add vertices to mesh
    for (const Vertex& v : cube_vertices)
        vertices.push_back(v);

    // Indicate how vertices are interconnected
    u32 cube_indices[36] =
    {
        // front face
        0, 1, 2,
        0, 2, 3,

        // back face
        4, 7, 5,
        7, 6, 5,

        // left face
        4, 5, 1,
        4, 1, 0,

        // right face
        3, 2, 6,
        3, 6, 7,

        // top face
        1, 5, 6,
        1, 6, 2,

        // bottom face
        4, 0, 3,
        4, 3, 7
    };

    // Add indices to mesh
    for (u16 i : cube_indices)
        indices.push_back(i);    
}
