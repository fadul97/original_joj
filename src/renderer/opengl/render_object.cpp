#include "renderer/opengl/render_object.h"

#include "resources/geometry/cube.h"
#include "resources/geometry/cylinder.h"
#include "resources/geometry/geo_sphere.h"
#include "resources/geometry/grid.h"
#include "resources/geometry/quad.h"
#include "resources/geometry/sphere.h"

joj::RenderObject::RenderObject(joj::GeometryType type)
{
    switch (type)
    {
    case GeometryType::CUBE: geometry = Cube{1.0f, 1.0f, 1.0f}; break;
    case GeometryType::CYLINDER: geometry = Cylinder{1.0f, 0.5f, 3.0f, 20, 10}; break;
    case GeometryType::SPHERE: geometry = Sphere{0.5f, 40, 40}; break;
    case GeometryType::GEOSPHERE: geometry = GeoSphere{1.0f, 3}; break;
    case GeometryType::GRID: geometry = Grid{100.0f, 20.0f, 20, 20}; break;
    case GeometryType::QUAD: geometry = Quad{1.0f, 1.0f}; break;
    case GeometryType::UNKNOWN: geometry = Cube{1.0f, 1.0f, 1.0f}; break;
    }

    vbo_id = 0;
    ebo_id = 0;
}

joj::RenderObject::~RenderObject()
{
    
}
