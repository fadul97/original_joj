#include "resources/mesh.h"

joj::Mesh::Mesh() {
    m_position = {
        .x = 0.0f,
        .y = 0.0f,
        .z = 0.0f
    };
}

joj::Mesh::~Mesh() = default;