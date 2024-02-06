#include <GL/glcorearb.h>
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
#include "joj/platform/x11/input_x11.h"
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
#include "joj/vendor/stb/stb_image.h"
#include "joj/renderer/opengl/texture_gl.h"

int main()
{
    auto pm = new joj::X11PlatformManager();
    pm->init(800, 600);
    pm->create_window();

    joj::GLRenderer renderer{};

    joj::GLShader shader{"shaders/vertex.glsl", "shaders/frag.glsl"};

    f32 vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };

    u32 indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    joj::VAO vao{};
    vao.bind();

    u32 bind_index = 0;
    vao.specify_position_data(bind_index, 3, 0);
    vao.specify_color_data(bind_index, 3, 3 * sizeof(f32));
    vao.specify_texture_data(bind_index, 2, 6 * sizeof(f32));

    u32 vbo = vao.create_vbo();
    u32 ebo = vao.create_ebo();
    vao.bind_vbo_data(vbo, sizeof(vertices), vertices);
    vao.bind_ebo_data(ebo, sizeof(indices), indices);

    vao.unbind_vbo();
    vao.unbind();
    
    stbi_set_flip_vertically_on_load(true);
    joj::GLTexture t1{"textures/container.jpg"};

    i32 width;
    i32 height;
    i32 nr_channels;
    unsigned char* data = stbi_load("textures/awesomeface.png", &width, &height, &nr_channels, 0);

    joj::GLTexture t2{};
    t2.create(width, height, data, GL_RGBA, GL_RGBA);

    stbi_image_free(data);

    renderer.init(pm->get_window());

    shader.use();
    shader.set_int("texture1", 0);
    shader.set_int("texture2", 1);

    while (pm->is_running())
    {
        pm->process_events();

        if (pm->is_key_pressed(KEY_ESCAPE))
            pm->close_window();
        
        renderer.clear();

        glActiveTexture(GL_TEXTURE0);
        t1.bind();
        glActiveTexture(GL_TEXTURE1);
        t2.bind();
        shader.use();

        vao.bind();
        vao.bind_vbo(bind_index, vbo, 0, 8 * sizeof(f32));
        vao.bind_ebo(ebo);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        vao.unbind();

        pm->swap_buffers();
    }

    pm->shutdown();
    delete pm;
    
    return 0;
}
