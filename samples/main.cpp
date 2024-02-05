#include <iostream>
#include <memory>

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
#include "joj/renderer/opengl/render_object.h"

int main()
{
    auto pm = new joj::X11PlatformManager();
    pm->init(800, 600);
    pm->create_window();

    joj::GLRenderer renderer{};

    joj::GLShader shader{"shaders/vertex.glsl", "shaders/frag.glsl"};

    auto quad = std::make_unique<joj::RenderObject>(joj::GeometryType::QUAD);
    auto cylinder = std::make_unique<joj::RenderObject>(joj::GeometryType::CYLINDER);
    auto geosphere = std::make_unique<joj::RenderObject>(joj::GeometryType::GEOSPHERE);
    auto cube = std::make_unique<joj::RenderObject>(joj::GeometryType::CUBE);
    
    cylinder->geometry.move_to(0, -5, 0);
    geosphere->geometry.move_to(-5, -5, 0);
    cube->geometry.move_to(+5, -5, 0);
    
    joj::VAO vao{};
    vao.bind();

    u32 bind_index = 0;
    vao.specify_position_data(bind_index, 3, 0);
    vao.specify_color_data(bind_index, 4, 3 * sizeof(f32));

    quad->vbo_id = vao.create_vbo();
    quad->ebo_id = vao.create_ebo();
    vao.bind_buffer_data(quad->vbo_id, quad->geometry.get_vertex_count() * sizeof(joj::Vertex), quad->geometry.get_vertex_data());
    vao.bind_buffer_data(quad->ebo_id, quad->geometry.get_index_count() * sizeof(u32), quad->geometry.get_index_data());

    cylinder->vbo_id = vao.create_vbo();
    cylinder->ebo_id = vao.create_ebo();
    vao.bind_buffer_data(cylinder->vbo_id, cylinder->geometry.get_vertex_count() * sizeof(joj::Vertex), cylinder->geometry.get_vertex_data());
    vao.bind_buffer_data(cylinder->ebo_id, cylinder->geometry.get_index_count() * sizeof(u32), cylinder->geometry.get_index_data());

    geosphere->vbo_id = vao.create_vbo();
    geosphere->ebo_id = vao.create_ebo();
    vao.bind_buffer_data(geosphere->vbo_id, geosphere->geometry.get_vertex_count() * sizeof(joj::Vertex), geosphere->geometry.get_vertex_data());
    vao.bind_buffer_data(geosphere->ebo_id, geosphere->geometry.get_index_count() * sizeof(u32), geosphere->geometry.get_index_data());

    cube->vbo_id = vao.create_vbo();
    cube->ebo_id = vao.create_ebo();
    vao.bind_buffer_data(cube->vbo_id, cube->geometry.get_vertex_count() * sizeof(joj::Vertex), cube->geometry.get_vertex_data());
    vao.bind_buffer_data(cube->ebo_id, cube->geometry.get_index_count() * sizeof(u32), cube->geometry.get_index_data());

    vao.unbind_vbo();
    vao.unbind();
    
    joj::Matrix4 world = joj::Matrix4{};
    f32 x = -005.0f;
    f32 y = +005.0f;
    f32 z = -000.0f;
    quad->geometry.move_to(x, y, z);

    joj::Vector3 pos = joj::Vector3{0, 0, -20};
    joj::Vector3 target = joj::Vector3{0};
    joj::Vector3 up = joj::Vector3{0, 1, 0};
    joj::Matrix4 view = joj::look_at_lh(pos, target, up);

    joj::Matrix4 proj = joj::perspective_lh(joj::to_radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);
    joj::Matrix4 transform = world * view * proj;

    f32 sz = -000.0f;
    f32 sx = +005.0f;
    geosphere->geometry.move_to(sx, +00.0f, sz);

    f32 tz = -010.0f;
    f32 tx = +000.0f;
    cylinder->geometry.move_to(tx, +00.0f, tz);

    renderer.init(pm->get_window());

    f32 mx = 0.0f;
    f32 my = 0.0f;
    f32 mz = 0.0f;
    
    // std::array<std::unique_ptr<joj::RenderObject>, 4> objects{std::move(quad), std::move(cylinder), std::move(geosphere), std::move(cube)};
    std::vector<std::unique_ptr<joj::RenderObject>> objects{};
    objects.push_back(std::move(quad));
    objects.push_back(std::move(cylinder));
    objects.push_back(std::move(geosphere));
    objects.push_back(std::move(cube));

    while (pm->is_running())
    {
        pm->process_events();

        if (pm->is_key_pressed(KEY_ESCAPE))
            pm->close_window();

        if (pm->is_key_down(KEY_D))
            mx++;
        if (pm->is_key_down(KEY_A))
            mx--;
        if (pm->is_key_down(KEY_W))
            my++;
        if (pm->is_key_down(KEY_S))
            my--;
        if (pm->is_key_down(KEY_F))
            mz++;
        if (pm->is_key_down(KEY_B))
            mz--;

        objects[3]->geometry.move_to(mx, my, mz);
        
        renderer.clear();

        vao.bind();
        shader.use();

        for (auto const& obj : objects)
        {
            world = joj::identity();
            world = joj::translate(world, obj->geometry.get_position());
            transform = world * view * proj;
            shader.set_mat4("transform", transform);
            vao.bind_vbo(bind_index, obj->vbo_id, 0, sizeof(joj::Vertex));
            vao.bind_ebo(obj->ebo_id);
            glDrawElements(GL_TRIANGLES, obj->geometry.get_index_count(), GL_UNSIGNED_INT, 0);
        }

        vao.unbind();

        pm->swap_buffers();
    }

    pm->shutdown();
    delete pm;
    
    return 0;
}
