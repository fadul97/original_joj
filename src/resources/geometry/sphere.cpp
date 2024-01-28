#include "resources/geometry/sphere.h"

joj::Sphere::Sphere()
{
    type = GeometryType::SPHERE;
}

joj::Sphere::Sphere(f32 radius, f32 slice_count, u32 layer_count)
{
    Vertex top_vertex;
    top_vertex.pos = Vector3{0.0f, radius, 0.0f};
    top_vertex.color = Vector4{1.0f, 1.0f, 0.0f, 1.0f};

    Vertex bottom_vertex;
    bottom_vertex.pos = Vector3{0.0f, -radius, 0.0f};
    bottom_vertex.color = Vector4{1.0f, 1.0f, 0.0f, 1.0f};

    vertices.push_back(top_vertex);

    f32 phi_step = J_PI / layer_count;
    f32 theta_step = 2.0f * J_PI / slice_count; 

    // Calculate the vertices for each ring (does not count the poles as rings)
    for (u32 i = 1; i <= layer_count - 1; ++i)
    {
        f32 phi = i * phi_step;

        // Ring vertices
        for (u32 j = 0; j <= slice_count; ++j)
        {
            f32 theta = j * theta_step;

            Vertex v;

            // Spherical coordinates for Cartesian coordinates
            v.pos.x = radius * sinf(phi) * cosf(theta);
            v.pos.y = radius * cosf(phi);
            v.pos.z = radius * sinf(phi) * sinf(theta);

            v.color = Vector4{1.0f, 1.0f, 0.0f, 1.0f};

            vertices.push_back(v);
        }
    }

    vertices.push_back(bottom_vertex);

    // Calculate the indexes of the top layer
    // This layer connects the top pole to the first ring
    for (u32 i = 1; i <= slice_count; ++i)
    {
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back(i);
    }

    // Calculate the indexes for the inner layers (not connected to the poles)
    u32 base_index = 1;
    u32 ring_vertex_count = slice_count + 1;
    for (u32 i = 0; i < layer_count - 2; ++i)
    {
        for (u32 j = 0; j < slice_count; ++j)
        {
            indices.push_back(base_index + i * ring_vertex_count + j);
            indices.push_back(base_index + i * ring_vertex_count + j + 1);
            indices.push_back(base_index + (i + 1) * ring_vertex_count + j);

            indices.push_back(base_index + (i + 1) * ring_vertex_count + j);
            indices.push_back(base_index + i * ring_vertex_count + j + 1);
            indices.push_back(base_index + (i + 1) * ring_vertex_count + j + 1);
        }
    }

    // Bottom pole is added last
    u32 south_pole_index = (u32)vertices.size() - 1;

    // It's positioned at the indices of the first vertex of the last ring
    base_index = south_pole_index - ring_vertex_count;

    for (u32 i = 0; i < slice_count; ++i)
    {
        indices.push_back(south_pole_index);
        indices.push_back(base_index + i);
        indices.push_back(base_index + i + 1);
    }
}

joj::Sphere::Sphere(f32 radius, f32 slice_count, u32 layer_count, Vector4 color)
{
    Vertex top_vertex;
    top_vertex.pos = Vector3{0.0f, radius, 0.0f};
    top_vertex.color = color;

    Vertex bottom_vertex;
    bottom_vertex.pos = Vector3{0.0f, -radius, 0.0f};
    bottom_vertex.color = color;

    vertices.push_back(top_vertex);

    f32 phi_step = J_PI / layer_count;
    f32 theta_step = 2.0f * J_PI / slice_count; 

    // Calculate the vertices for each ring (does not count the poles as rings)
    for (u32 i = 1; i <= layer_count - 1; ++i)
    {
        f32 phi = i * phi_step;

        // Ring vertices
        for (u32 j = 0; j <= slice_count; ++j)
        {
            f32 theta = j * theta_step;

            Vertex v;

            // Spherical coordinates for Cartesian coordinates
            v.pos.x = radius * sinf(phi) * cosf(theta);
            v.pos.y = radius * cosf(phi);
            v.pos.z = radius * sinf(phi) * sinf(theta);

            v.color = color;

            vertices.push_back(v);
        }
    }

    vertices.push_back(bottom_vertex);

    // Calculate the indexes of the top layer
    // This layer connects the top pole to the first ring
    for (u32 i = 1; i <= slice_count; ++i)
    {
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back(i);
    }

    // Calculate the indexes for the inner layers (not connected to the poles)
    u32 base_index = 1;
    u32 ring_vertex_count = slice_count + 1;
    for (u32 i = 0; i < layer_count - 2; ++i)
    {
        for (u32 j = 0; j < slice_count; ++j)
        {
            indices.push_back(base_index + i * ring_vertex_count + j);
            indices.push_back(base_index + i * ring_vertex_count + j + 1);
            indices.push_back(base_index + (i + 1) * ring_vertex_count + j);

            indices.push_back(base_index + (i + 1) * ring_vertex_count + j);
            indices.push_back(base_index + i * ring_vertex_count + j + 1);
            indices.push_back(base_index + (i + 1) * ring_vertex_count + j + 1);
        }
    }

    // Bottom pole is added last
    u32 south_pole_index = (u32)vertices.size() - 1;

    // It's positioned at the indices of the first vertex of the last ring
    base_index = south_pole_index - ring_vertex_count;

    for (u32 i = 0; i < slice_count; ++i)
    {
        indices.push_back(south_pole_index);
        indices.push_back(base_index + i);
        indices.push_back(base_index + i + 1);
    }
}

