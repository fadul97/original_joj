#include "resources/geometry/geo_sphere.h"

#include "math/jmath.h"

joj::GeoSphere::GeoSphere()
{
    type = GeometryType::GEOSPHERE;
}


joj::GeoSphere::GeoSphere(f32 radius, u32 subdivisions)
{

    type = GeometryType::GEOSPHERE;

    // Max number of subdivisions is 6
    subdivisions = (subdivisions > 6U ? 6U : subdivisions);

    // Approximate a sphere by the subdivision of an icosahedron
    const f32 X = 0.525731f;
    const f32 Z = 0.850651f;

    // Vertices of the icosahedron
    Vector3 pos[12] =
    {
        Vector3{-X, 0.0f, Z},  Vector3{X, 0.0f, Z},
        Vector3{-X, 0.0f, -Z}, Vector3{X, 0.0f, -Z},
        Vector3{0.0f, Z, X},   Vector3{0.0f, Z, -X},
        Vector3{0.0f, -Z, X},  Vector3{0.0f, -Z, -X},
        Vector3{Z, X, 0.0f},   Vector3{-Z, X, 0.0f},
        Vector3{Z, -X, 0.0f},  Vector3{-Z, -X, 0.0f}
    };

    // Indices of the icosahedron
    u32 k[60] =
    {
        1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
        1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
        3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
        10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
    };

    // Snap and initialize vertex vectors and indices
    vertices.resize(12);
    indices.assign(&k[0], &k[60]);

    for (u32 i = 0; i < 12; ++i)
        vertices[i].pos = pos[i];

    // Subdivide each triangle of the icosahedron a certain number of times
    for (u32 i = 0; i < subdivisions; ++i)
        subdivide();

    // Project the vertices onto a sphere and adjust the scale
    for (u32 i = 0; i < vertices.size(); ++i)
    {
        // Normalize vetor (point)
        Vector3 n = normalize(vertices[i].pos);

        // Project on sphere
        vertices[i].pos = n * radius;

        vertices[i].color = Vector4{ 1.0f, 1.0f, 0.0f, 1.0f };
    }
}

joj::GeoSphere::GeoSphere(f32 radius, u32 subdivisions, Vector4 color)
{
    type = GeometryType::GEOSPHERE;

    // Max number of subdivisions is 6
    subdivisions = (subdivisions > 6U ? 6U : subdivisions);

    // Approximate a sphere by the subdivision of an icosahedron
    const f32 X = 0.525731f;
    const f32 Z = 0.850651f;

    // Vertices of the icosahedron
    Vector3 pos[12] =
    {
        Vector3{-X, 0.0f, Z},  Vector3{X, 0.0f, Z},
        Vector3{-X, 0.0f, -Z}, Vector3{X, 0.0f, -Z},
        Vector3{0.0f, Z, X},   Vector3{0.0f, Z, -X},
        Vector3{0.0f, -Z, X},  Vector3{0.0f, -Z, -X},
        Vector3{Z, X, 0.0f},   Vector3{-Z, X, 0.0f},
        Vector3{Z, -X, 0.0f},  Vector3{-Z, -X, 0.0f}
    };

    // Indices of the icosahedron
    u32 k[60] =
    {
        1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
        1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
        3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
        10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
    };

    // Snap and initialize vertex vectors and indices
    vertices.resize(12);
    indices.assign(&k[0], &k[60]);

    for (u32 i = 0; i < 12; ++i)
        vertices[i].pos = pos[i];

    // Subdivide each triangle of the icosahedron a certain number of times
    for (u32 i = 0; i < subdivisions; ++i)
        subdivide();

    // Project the vertices onto a sphere and adjust the scale
    for (u32 i = 0; i < vertices.size(); ++i)
    {
        // Normalize vetor (point)
        Vector3 n = normalize(vertices[i].pos);

        // Project on sphere
        vertices[i].pos = n * radius;

        vertices[i].color = color;
    }
}
