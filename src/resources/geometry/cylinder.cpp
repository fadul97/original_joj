#include "resources/geometry/cylinder.h"

joj::Cylinder::Cylinder()
{
    type = GeometryType::CYLINDER;
}

joj::Cylinder::Cylinder(f32 bottom, f32 top, f32 height, u32 slice_count, u32 layer_count)
{
    type = GeometryType::CYLINDER;

    // Layer height
    f32 layer_height = height / layer_count;

    // Increment in the radius of each layer
    f32 radius_step = (top - bottom) / layer_count;

    // Number of cylinder rings
    u32 ring_count = layer_count + 1;

    // Calculate vertices of each ring
    for (u32 i = 0; i < ring_count; ++i)
    {
        f32 y = -0.5f * height + i * layer_height;
        f32 r = bottom + i * radius_step;
        f32 theta = 2.0f * J_PI / slice_count;

        for (u32 j = 0; j <= slice_count; ++j)
        {
            f32 c = cosf(j * theta);
            f32 s = sinf(j * theta);

            Vertex vertex;
            vertex.pos = Vector3{r * c, y, r * s};
            vertex.color = Vector4{1.0f, 1.0f, 0.0f, 1.0f};
            vertices.push_back(vertex);
        }
    }

    // Number of vertices in each cylinder ring
    u32 ring_vertex_count = slice_count + 1;

    // Calculate indexes for each layer
    for (u32 i = 0; i < layer_count; ++i)
    {
        for (u32 j = 0; j < slice_count; ++j)
        {
            indices.push_back(i * ring_vertex_count + j);
            indices.push_back((i + 1) * ring_vertex_count + j);
            indices.push_back((i + 1) * ring_vertex_count + j + 1);
            indices.push_back(i * ring_vertex_count + j);
            indices.push_back((i + 1) * ring_vertex_count + j + 1);
            indices.push_back(i * ring_vertex_count + j + 1);
        }
    }

    // Constructs vertices of cylinder covers
    for (u32 k = 0; k < 2; ++k)
    {
        u32 base_index = u32(vertices.size());

        f32 y = (k - 0.5f) * height;
        f32 theta = 2.0f * J_PI / slice_count;
        f32 r = (k ? top : bottom);
        Vertex vertex;

        for (u32 i = 0; i <= slice_count; i++)
        {
            f32 x = r * cosf(i * theta);
            f32 z = r * sinf(i * theta);

            vertex.pos = Vector3{x, y, z};
            vertex.color = Vector4{1.0f, 1.0f, 0.0f, 1.0f};
            vertices.push_back(vertex);
        }

        // Central vertex of the lid
        vertex.pos = Vector3{0.0f, y, 0.0f};
        vertex.color = Vector4{1.0f, 1.0f, 0.0f, 1.0f};
        vertices.push_back(vertex);

        u32 center_index = u32(vertices.size() - 1);

        // Indices for the lid
        for (u32 i = 0; i < slice_count; ++i)
        {
            indices.push_back(center_index);
            indices.push_back(base_index + i + k);
            indices.push_back(base_index + i + 1 - k);
        }
    }
}

joj::Cylinder::Cylinder(f32 bottom, f32 top, f32 height, u32 slice_count, u32 layer_count, Vector4 color)
{
    type = GeometryType::CYLINDER;

    // Layer height
    f32 layer_height = height / layer_count;

    // Increment in the radius of each layer
    f32 radius_step = (top - bottom) / layer_count;

    // Number of cylinder rings
    u32 ring_count = layer_count + 1;

    // Calculate vertices of each ring
    for (u32 i = 0; i < ring_count; ++i)
    {
        f32 y = -0.5f * height + i * layer_height;
        f32 r = bottom + i * radius_step;
        f32 theta = 2.0f * J_PI / slice_count;

        for (u32 j = 0; j <= slice_count; ++j)
        {
            f32 c = cosf(j * theta);
            f32 s = sinf(j * theta);

            Vertex vertex;
            vertex.pos = Vector3{r * c, y, r * s};
            vertex.color = color;
            vertices.push_back(vertex);
        }
    }

    // Number of vertices in each cylinder ring
    u32 ring_vertex_count = slice_count + 1;

    // Calculate indexes for each layer
    for (u32 i = 0; i < layer_count; ++i)
    {
        for (u32 j = 0; j < slice_count; ++j)
        {
            indices.push_back(i * ring_vertex_count + j);
            indices.push_back((i + 1) * ring_vertex_count + j);
            indices.push_back((i + 1) * ring_vertex_count + j + 1);
            indices.push_back(i * ring_vertex_count + j);
            indices.push_back((i + 1) * ring_vertex_count + j + 1);
            indices.push_back(i * ring_vertex_count + j + 1);
        }
    }

    // Constructs vertices of cylinder covers
    for (u32 k = 0; k < 2; ++k)
    {
        u32 base_index = u32(vertices.size());

        f32 y = (k - 0.5f) * height;
        f32 theta = 2.0f * J_PI / slice_count;
        f32 r = (k ? top : bottom);
        Vertex vertex;

        for (u32 i = 0; i <= slice_count; i++)
        {
            f32 x = r * cosf(i * theta);
            f32 z = r * sinf(i * theta);

            vertex.pos = Vector3{x, y, z};
            vertex.color = color;
            vertices.push_back(vertex);
        }

        // Central vertex of the lid
        vertex.pos = Vector3{0.0f, y, 0.0f};
        vertex.color = color;
        vertices.push_back(vertex);

        u32 center_index = u32(vertices.size() - 1);

        // Indices for the lid
        for (u32 i = 0; i < slice_count; ++i)
        {
            indices.push_back(center_index);
            indices.push_back(base_index + i + k);
            indices.push_back(base_index + i + 1 - k);
        }
    }
}
