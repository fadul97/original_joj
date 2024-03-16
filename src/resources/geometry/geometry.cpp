#include "resources/geometry/geometry.h"

joj::Geometry::Geometry()
    : position(0.0f, 0.0f, 0.0f), type(GeometryType::UNKNOWN)
{
}

joj::Geometry::~Geometry()
{
}

#if JPLATFORM_WINDOWS
using namespace DirectX;

void joj::Geometry::subdivide()
{
    // Save a copy of the original geometry
    std::vector <Vertex> vertices_copy = vertices;
    std::vector <u32> indices_copy = indices;

    vertices.resize(0);
    indices.resize(0);

    //       v1
    //       *
    //      / \
    //     /   \
    //  m0*-----*m1
    //   / \   / \
    //  /   \ /   \
    // *-----*-----*
    // v0    m2     v2

    u32 num_tris = static_cast<u32>(indices_copy.size() / 3);

    for (u32 i = 0; i < num_tris; ++i)
    {
        Vertex v0 = vertices_copy[indices_copy[size_t(i) * 3 + 0]];
        Vertex v1 = vertices_copy[indices_copy[size_t(i) * 3 + 1]];
        Vertex v2 = vertices_copy[indices_copy[size_t(i) * 3 + 2]];

        // Find center points of each edge
        Vertex m0, m1, m2;
        XMStoreFloat3(&m0.pos, 0.5f * (XMLoadFloat3(&v0.pos) + XMLoadFloat3(&v1.pos)));
        XMStoreFloat3(&m1.pos, 0.5f * (XMLoadFloat3(&v1.pos) + XMLoadFloat3(&v2.pos)));
        XMStoreFloat3(&m2.pos, 0.5f * (XMLoadFloat3(&v0.pos) + XMLoadFloat3(&v2.pos)));

        // Add new geometry
        vertices.push_back(v0); // 0
        vertices.push_back(v1); // 1
        vertices.push_back(v2); // 2
        vertices.push_back(m0); // 3
        vertices.push_back(m1); // 4
        vertices.push_back(m2); // 5

        indices.push_back(i * 6 + 0);
        indices.push_back(i * 6 + 3);
        indices.push_back(i * 6 + 5);

        indices.push_back(i * 6 + 3);
        indices.push_back(i * 6 + 4);
        indices.push_back(i * 6 + 5);

        indices.push_back(i * 6 + 5);
        indices.push_back(i * 6 + 4);
        indices.push_back(i * 6 + 2);

        indices.push_back(i * 6 + 3);
        indices.push_back(i * 6 + 1);
        indices.push_back(i * 6 + 4);
    }
}
#else
void joj::Geometry::subdivide()
{
    // Save a copy of the original geometry
    std::vector <Vertex> vertices_copy = vertices;
    std::vector <u32> indices_copy = indices;

    vertices.resize(0);
    indices.resize(0);

    //       v1
    //       *
    //      / \
    //     /   \
    //  m0*-----*m1
    //   / \   / \
    //  /   \ /   \
    // *-----*-----*
    // v0    m2     v2

    u32 num_tris = static_cast<u32>(indices_copy.size() / 3);

    for (u32 i = 0; i < num_tris; ++i)
    {
        Vertex v0 = vertices_copy[indices_copy[size_t(i) * 3 + 0]];
        Vertex v1 = vertices_copy[indices_copy[size_t(i) * 3 + 1]];
        Vertex v2 = vertices_copy[indices_copy[size_t(i) * 3 + 2]];

        // Find center points of each edge
        Vertex m0, m1, m2;
        m0.pos = (v0.pos + v1.pos) * 0.5f;
        m1.pos = (v1.pos + v2.pos) * 0.5f;
        m2.pos = (v0.pos + v2.pos) * 0.5f;

        // Add new geometry
        vertices.push_back(v0); // 0
        vertices.push_back(v1); // 1
        vertices.push_back(v2); // 2
        vertices.push_back(m0); // 3
        vertices.push_back(m1); // 4
        vertices.push_back(m2); // 5

        indices.push_back(i * 6 + 0);
        indices.push_back(i * 6 + 3);
        indices.push_back(i * 6 + 5);

        indices.push_back(i * 6 + 3);
        indices.push_back(i * 6 + 4);
        indices.push_back(i * 6 + 5);

        indices.push_back(i * 6 + 5);
        indices.push_back(i * 6 + 4);
        indices.push_back(i * 6 + 2);

        indices.push_back(i * 6 + 3);
        indices.push_back(i * 6 + 1);
        indices.push_back(i * 6 + 4);
    }
}
#endif