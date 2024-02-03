#include <iostream>

#include "joj/joj.h"
#include "joj/defines.h"
#include "joj/platform/window.h"
#include "joj/platform/x11/window_x11.h"
#include "joj/platform/platform_manager.h"
#include "joj/platform/x11/platform_manager_x11.h"
#include "joj/platform/input.h"
#include "joj/platform/timer.h"
#include "joj/platform/x11/timer_x11.h"
#include "joj/graphics/context.h"
#include "joj/graphics/x11/context_gl.h"
#include "joj/graphics/x11/joj_gl_x11.h"
#include "joj/math/vector3.h"
#include "joj/math/vector4.h"
#include "joj/math/matrix4.h"
#include "joj/renderer/renderer.h"
#include "joj/renderer/opengl/renderer_gl.h"
#include "joj/math/jmath.h"
#include "platform/x11/input_x11.h"
#include "joj/renderer/shader.h"
#include "joj/renderer/opengl/shader_gl.h"
#include "joj/resources/geometry/geometry.h"
#include "joj/resources/geometry/cube.h"
#include "joj/resources/geometry/sphere.h"
#include "joj/resources/geometry/cylinder.h"
#include "joj/resources/geometry/geo_sphere.h"
#include "joj/resources/geometry/grid.h"
#include "joj/resources/geometry/quad.h"
#include "joj/renderer/opengl/vao.h"
#include "joj/resources/geometry/vertex.h"

int main()
{
    auto pm = new joj::X11PlatformManager();
    pm->init(800, 600);
    pm->create_window();

    joj::GLRenderer renderer{};

    joj::GLShader shader{"shaders/vertex.glsl", "shaders/frag.glsl"};

    // joj::Cube geo{1.0f, 1.0f, 1.0f};
    // joj::Sphere geo{0.5f, 40, 40};
    // joj::Cylinder geo{1.0f, 0.5f, 3.0f, 20, 10};
    // joj::GeoSphere geo{1.0f, 3};
    // joj::Grid geo{100.0f, 20.0f, 20, 20};
    joj::Quad geo{05.0f, 05.0f};
    
    joj::VAO vao = joj::VAO{};
    vao.bind_buffer_data(
        geo.get_vertex_count() * sizeof(joj::Vertex),
        geo.get_vertex_data(),
        geo.get_index_count() * sizeof(u32),
        geo.get_index_data()
    );

    joj::Matrix4 world = joj::Matrix4{};
    f32 z = -000.0f;
    f32 x = -000.0f;
    geo.move_to(x, +00.0f, z);
    world = joj::translate(world, geo.get_position());

    joj::Vector3 pos = joj::Vector3{0, 0, -20};
    joj::Vector3 target = joj::Vector3{0};
    joj::Vector3 up = joj::Vector3{0, 1, 0};
    joj::Matrix4 view = joj::look_at_lh(pos, target, up);

    joj::Matrix4 proj = joj::perspective_lh(joj::to_radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    
    joj::Matrix4 transform = world * view * proj;
    shader.set_mat4("transform", transform);

    renderer.init(pm->get_window());
    
    while (pm->is_running())
    {
        pm->process_events();

        if (pm->is_key_pressed(KEY_ESCAPE))
            pm->close_window();
        
        renderer.clear();

        transform = world * view * proj;

        shader.use();
        shader.set_mat4("transform", transform);
        vao.bind();
        glDrawElements(GL_TRIANGLES, geo.get_index_count(), GL_UNSIGNED_INT, 0);

        pm->swap_buffers();
    }

    pm->shutdown();
    delete pm;
    
    return 0;
}
