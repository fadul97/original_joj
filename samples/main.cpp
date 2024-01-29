#include <GL/glcorearb.h>
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


const char *vertexShaderSource = "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
			"layout (location = 1) in vec4 aColor;\n"
            "uniform mat4 transform;\n"
            "out vec4 vertColor;\n"
            "void main()\n"
            "{\n"
            "    gl_Position = transform * vec4(aPos, 1.0);\n"
            "    vertColor = aColor;\n"
            "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
            "in vec4 vertColor;\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            "   FragColor = vertColor;\n"
            "}\n\0";
unsigned int VAO = 0;

int main()
{
    auto pm = new joj::X11PlatformManager();
    pm->init(800, 600);
    pm->create_window();

    auto renderer = new joj::GLRenderer();

    joj::GLShader shader{};
    if (!shader.compile_shaders(vertexShaderSource, fragmentShaderSource))
        std::cout << "Failed to compile shaders.\n";

    // joj::Cube geo{1.0f, 1.0f, 1.0f};
    // joj::Sphere geo{0.5f, 40, 40};
    // joj::Cylinder geo{1.0f, 0.5f, 3.0f, 20, 10};
    // joj::GeoSphere geo{1.0f, 3};
    // joj::Grid geo{100.0f, 20.0f, 20, 20};
    joj::Quad geo{1.0f, 1.0f};
    
    unsigned int VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, geo.get_vertex_count() * sizeof(joj::Vertex), geo.get_vertex_data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, geo.get_index_count() * sizeof(u32), geo.get_index_data(), GL_STATIC_DRAW);

    // Specify the layout of the vertex(pos) data
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(joj::Vertex), (void*)0);

    // Specify the layout of the vertex(color) data
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(joj::Vertex), (GLvoid*)(3 * sizeof(f32)));

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // -----------------------------------------------------------------------------------------------------------------

    joj::Matrix4 world = joj::Matrix4{};
    geo.move_to(+00.0f, +00.0f, -10.0f);
    world = joj::translate(world, geo.get_position());

    joj::Vector3 pos = joj::Vector3{0, 0, -20};
    joj::Vector3 target = joj::Vector3{0};
    joj::Vector3 up = joj::Vector3{0, 1, 0};
    joj::Matrix4 view = joj::look_at_lh(pos, target, up);

    joj::Matrix4 proj = joj::perspective_lh(joj::to_radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    
    // joj::Matrix4 transform = proj * view * world;
    joj::Matrix4 transform = world * view * proj;
    shader.set_mat4("transform", transform);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    while (pm->is_running())
    {
        pm->process_events();

        if (pm->is_key_pressed(KEY_SPACE))
            std::cout << "KEY_SPACE!\n";

        if (pm->is_key_down(KEY_ENTER) && pm->is_key_down(KEY_A))
            std::cout << "KEY_ENTER!\n";

        if (pm->is_key_pressed(KEY_ESCAPE))
            pm->close_window();
        
        renderer->clear();

        shader.use();
        // cube.translate(0.0f, 0.0f, -0.005f);
        // world = joj::translate(world, cube.get_position());
        joj::Matrix4 transform = world * view * proj;
        shader.set_mat4("transform", transform);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, geo.get_index_count(), GL_UNSIGNED_INT, 0);

        pm->swap_buffers();
    }

    pm->shutdown();

    delete renderer;
    delete pm;
    
    return 0;
}
