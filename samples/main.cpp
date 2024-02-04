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
    joj::GLShader s2{"shaders/vertex.glsl", "shaders/frag.glsl"};

    // joj::Cube geo{1.0f, 1.0f, 1.0f};
    // joj::Sphere geo{0.5f, 40, 40};
    joj::Cylinder geoc{1.0f, 0.5f, 3.0f, 20, 10};
    joj::GeoSphere geos{1.0f, 3};
    // joj::Grid geo{100.0f, 20.0f, 20, 20};
    joj::Quad geo{03.0f, 03.0f};
    
    u32 vao;
    u32 vbo;
    u32 ebo;

    u32 svbo;
    u32 sebo;

    u32 tvbo;
    u32 tebo;

    // Generate buffers
    glCreateVertexArrays(1, &vao);
    glCreateBuffers(1, &vbo);
    glCreateBuffers(1, &ebo);

    glCreateBuffers(1, &svbo);
    glCreateBuffers(1, &sebo);

    glCreateBuffers(1, &tvbo);
    glCreateBuffers(1, &tebo);

    // Bind vao
    glBindVertexArray(vao);
    
    // Setup buffer format
    i32 bind_index = 0;

    // Vertices
    u32 pos_location = 0;
    glEnableVertexAttribArray(pos_location);
    glVertexAttribFormat(pos_location, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(pos_location, bind_index);

    // Color
    u32 color_location = 1;
    glEnableVertexAttribArray(color_location);
    glVertexAttribFormat(color_location, 4, GL_FLOAT, GL_FALSE, 3 * sizeof(f32));
    glVertexAttribBinding(color_location, bind_index);

    // Set buffer data
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, geo.get_vertex_count() * sizeof(joj::Vertex), geo.get_vertex_data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, geo.get_index_count() * sizeof(u32), geo.get_index_data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, svbo);
    glBufferData(GL_ARRAY_BUFFER, geos.get_vertex_count() * sizeof(joj::Vertex), geos.get_vertex_data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, geos.get_index_count() * sizeof(u32), geos.get_index_data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, tvbo);
    glBufferData(GL_ARRAY_BUFFER, geoc.get_vertex_count() * sizeof(joj::Vertex), geoc.get_vertex_data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, geoc.get_index_count() * sizeof(u32), geoc.get_index_data(), GL_STATIC_DRAW);


    
    // glBindVertexBuffer(bind_index, vbo, 0, 7 * sizeof(f32));
    // glBindVertexBuffer(bind_index, vbo, 0, sizeof(joj::Vertex));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    joj::Matrix4 world = joj::Matrix4{};
    f32 z = -000.0f;
    f32 x = -005.0f;
    geo.move_to(x, +00.0f, z);
    world = joj::translate(world, geo.get_position());

    joj::Vector3 pos = joj::Vector3{0, 0, -20};
    joj::Vector3 target = joj::Vector3{0};
    joj::Vector3 up = joj::Vector3{0, 1, 0};
    joj::Matrix4 view = joj::look_at_lh(pos, target, up);

    joj::Matrix4 proj = joj::perspective_lh(joj::to_radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    
    joj::Matrix4 transform = world * view * proj;
    shader.use();
    shader.set_mat4("transform", transform);

    joj::Matrix4 w2 = joj::Matrix4{};
    f32 sz = -000.0f;
    f32 sx = +005.0f;
    geos.move_to(sx, +00.0f, sz);
    w2 = joj::translate(w2, geos.get_position());

    joj::Matrix4 t2 = w2 * view * proj;
    s2.use();
    s2.set_mat4("transform", t2);

    joj::Matrix4 w3 = joj::Matrix4{};
    f32 tz = -010.0f;
    f32 tx = +000.0f;
    geoc.move_to(tx, +00.0f, tz);
    w3 = joj::translate(w3, geoc.get_position());

    renderer.init(pm->get_window());
    
    while (pm->is_running())
    {
        pm->process_events();

        if (pm->is_key_pressed(KEY_ESCAPE))
            pm->close_window();
        
        renderer.clear();

        // vao.bind();
        glBindVertexArray(vao);
        shader.use();

        transform = world * view * proj;
        shader.set_mat4("transform", transform);
        glBindVertexBuffer(bind_index, vbo, 0, sizeof(joj::Vertex));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glDrawElements(GL_TRIANGLES, geo.get_index_count(), GL_UNSIGNED_INT, 0);

        t2 = w2 * view * proj;
        shader.set_mat4("transform", t2);
        glBindVertexBuffer(bind_index, svbo, 0, sizeof(joj::Vertex));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sebo);
        glDrawElements(GL_TRIANGLES, geos.get_index_count(), GL_UNSIGNED_INT, 0);

        t2 = w3 * view * proj;
        shader.set_mat4("transform", t2);
        glBindVertexBuffer(bind_index, tvbo, 0, sizeof(joj::Vertex));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tebo);
        glDrawElements(GL_TRIANGLES, geoc.get_index_count(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        pm->swap_buffers();
    }

    pm->shutdown();
    delete pm;
    
    return 0;
}
