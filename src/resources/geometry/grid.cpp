#include "resources/geometry/grid.h"

joj::Grid::Grid()
{
    type = GeometryType::GRID;
}

#if JPLATFORM_WINDOWS
#include <DirectXColors.h>

using namespace DirectX;

joj::Grid::Grid(f32 width, f32 depth, u32 m, u32 n)
{
    type = GeometryType::GRID;

    u32 vertex_count = m * n;
    u32 triangle_count = 2 * (m - 1) * (n - 1);

    // Create vertices
    f32 half_width = 0.5f * width;
    f32 half_depth = 0.5f * depth;

    f32 dx = width / (n - 1);
    f32 dz = depth / (m - 1);

    // Adjust vertex vector size
    vertices.resize(vertex_count);

    for (u32 i = 0; i < m; ++i)
    {
        f32 z = half_depth - i * dz;

        for (u32 j = 0; j < n; ++j)
        {
            f32 x = -half_width + j * dx;

            // Define grid vertices
            vertices[size_t(i) * n + j].pos = Vector3(x, 0.0f, z);
            vertices[size_t(i) * n + j].color = Vector4(1.0f, 1.0f, 0.0f, 1.0f);
        }
    }

    // Adjust vector size of indexes
    indices.resize(size_t(triangle_count) * 3);

    size_t k = 0;

    for (u32 i = 0; i < m - 1; ++i)
    {
        for (u32 j = 0; j < n - 1; ++j)
        {
            indices[k] = i * n + j;
            indices[k + 1] = i * n + j + 1;
            indices[k + 2] = (i + 1) * n + j;
            indices[k + 3] = (i + 1) * n + j;
            indices[k + 4] = i * n + j + 1;
            indices[k + 5] = (i + 1) * n + j + 1;

            k += 6; // Next quad
        }
    }
}

joj::Grid::Grid(f32 width, f32 depth, u32 m, u32 n, Vector4 color)
{
    type = GeometryType::GRID;

    u32 vertex_count = m * n;
    u32 triangle_count = 2 * (m - 1) * (n - 1);

    // Create vertices
    f32 half_width = 0.5f * width;
    f32 half_depth = 0.5f * depth;

    f32 dx = width / (n - 1);
    f32 dz = depth / (m - 1);

    // Adjust vertex vector size
    vertices.resize(vertex_count);

    for (u32 i = 0; i < m; ++i)
    {
        f32 z = half_depth - i * dz;

        for (u32 j = 0; j < n; ++j)
        {
            f32 x = -half_width + j * dx;

            // Define grid vertices
            vertices[size_t(i) * n + j].pos = DirectX::XMFLOAT3(x, 0.0f, z);
            vertices[size_t(i) * n + j].color = color;
        }
    }

    // Adjust vector size of indexes
    indices.resize(size_t(triangle_count) * 3);

    size_t k = 0;

    for (u32 i = 0; i < m - 1; ++i)
    {
        for (u32 j = 0; j < n - 1; ++j)
        {
            indices[k] = i * n + j;
            indices[k + 1] = i * n + j + 1;
            indices[k + 2] = (i + 1) * n + j;
            indices[k + 3] = (i + 1) * n + j;
            indices[k + 4] = i * n + j + 1;
            indices[k + 5] = (i + 1) * n + j + 1;

            k += 6; // Next quad
        }
    }
}

#else
#include "math/jmath.h"

joj::Grid::Grid(f32 width, f32 depth, u32 m, u32 n)
{
    type = GeometryType::GRID;

    u32 vertex_count = m * n;
    u32 triangle_count = 2 * (m - 1) * (n - 1);

    // Create vertices
    f32 half_width = 0.5f * width;
    f32 half_depth = 0.5f * depth;

    f32 dx = width / (n - 1);
    f32 dz = depth / (m - 1);

    // Adjust vertex vector size
    vertices.resize(vertex_count);

    for (u32 i = 0; i < m; ++i)
    {
        f32 z = half_depth - i * dz;

        for (u32 j = 0; j < n; ++j)
        {
            f32 x = -half_width + j * dx;

            // Define grid vertices
            vertices[size_t(i) * n + j].pos = Vector3{x, 0.0f, z};
            vertices[size_t(i) * n + j].color = Vector4(1.0f, 1.0f, 0.0f, 1.0f);
        }
    }

    // Adjust vector size of indexes
    indices.resize(size_t(triangle_count) * 3);

    size_t k = 0;

    for (u32 i = 0; i < m - 1; ++i)
    {
        for (u32 j = 0; j < n - 1; ++j)
        {
            indices[k] = i * n + j;
            indices[k + 1] = i * n + j + 1;
            indices[k + 2] = (i + 1) * n + j;
            indices[k + 3] = (i + 1) * n + j;
            indices[k + 4] = i * n + j + 1;
            indices[k + 5] = (i + 1) * n + j + 1;

            k += 6; // Next quad
        }
    }
}

joj::Grid::Grid(f32 width, f32 depth, u32 m, u32 n, Vector4 color)
{
    type = GeometryType::GRID;

    u32 vertex_count = m * n;
    u32 triangle_count = 2 * (m - 1) * (n - 1);

    // Create vertices
    f32 half_width = 0.5f * width;
    f32 half_depth = 0.5f * depth;

    f32 dx = width / (n - 1);
    f32 dz = depth / (m - 1);

    // Adjust vertex vector size
    vertices.resize(vertex_count);

    for (u32 i = 0; i < m; ++i)
    {
        f32 z = half_depth - i * dz;

        for (u32 j = 0; j < n; ++j)
        {
            f32 x = -half_width + j * dx;

            // Define grid vertices
            vertices[size_t(i) * n + j].pos = Vector3{x, 0.0f, z};
            vertices[size_t(i) * n + j].color = color;
        }
    }

    // Adjust vector size of indexes
    indices.resize(size_t(triangle_count) * 3);

    size_t k = 0;

    for (u32 i = 0; i < m - 1; ++i)
    {
        for (u32 j = 0; j < n - 1; ++j)
        {
            indices[k] = i * n + j;
            indices[k + 1] = i * n + j + 1;
            indices[k + 2] = (i + 1) * n + j;
            indices[k + 3] = (i + 1) * n + j;
            indices[k + 4] = i * n + j + 1;
            indices[k + 5] = (i + 1) * n + j + 1;

            k += 6; // Next quad
        }
    }
}

#endif

