#include <iostream>
#include <memory>
#include <unistd.h>

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
#include "joj/renderer/opengl/animation2d_gl.h"
#include "joj/renderer/opengl/texture_rect_gl.h"
#include "joj/platform/icon_image.h"

double get_abs_time()
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC_RAW, &now);
    return now.tv_sec + now.tv_nsec * 0.000000001;
}

const f64 TARGET_FRAME_TIME = 1.0 / 60.0; // 60 FPS

int main()
{
    auto pm = new joj::X11PlatformManager();
    pm->init(800, 600);
    pm->create_window();

    joj::IconImage logo;
    logo.pixels = stbi_load("../logo.png", &logo.width, &logo.height, 0, 4);
    pm->set_window_icon(1, logo);

    joj::GLRenderer renderer{};

    joj::GLShader shader{"shaders/vertex.glsl", "shaders/frag.glsl"};

    stbi_set_flip_vertically_on_load(true);

    i32 width;
    i32 height;
    i32 nr_channels;
    // Texture from brullov (https://brullov.itch.io/)
    unsigned char* data = stbi_load("textures/char_purple.png", &width, &height, &nr_channels, 0);
    if (!data)
        std::cout << "Failed to load image file.\n";

    joj::GLTexture t2{};
    t2.create(width, height, data, GL_RGBA, GL_RGBA);
    stbi_image_free(data);

    joj::Vector2 bottom_left =  joj::Vector2{ 0.0f, 0.0f };
    joj::Vector2 bottom_right = joj::Vector2{ 1.0f, 0.0f };
    joj::Vector2 top_right =    joj::Vector2{ 1.0f, 1.0f };
    joj::Vector2 top_left =     joj::Vector2{ 0.0f, 1.0f };
    
    f32 vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   top_right.x, top_right.y,
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   bottom_right.x, bottom_right.y,
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   bottom_left.x, bottom_left.y,
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   top_left.x, top_left.y
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
    
    joj::GLTexture t1{"textures/container.jpg", GL_RGB, GL_RGB};

    renderer.init(pm->get_window());

    shader.use();
    shader.set_int("texture1", 0);
    shader.set_int("texture2", 1);

    joj::GLAnimation2D walking_right;
    walking_right.texture = &t2;
    walking_right.add_frame(joj::GLTextureRect{0, 8, 56, 56, 448, 616});
    walking_right.add_frame(joj::GLTextureRect{0, 8, 56, 56, 448, 616});
    walking_right.add_frame(joj::GLTextureRect{1, 8, 56, 56, 448, 616});
    walking_right.add_frame(joj::GLTextureRect{1, 8, 56, 56, 448, 616});
    walking_right.add_frame(joj::GLTextureRect{2, 8, 56, 56, 448, 616});
    walking_right.add_frame(joj::GLTextureRect{2, 8, 56, 56, 448, 616});
    walking_right.add_frame(joj::GLTextureRect{3, 8, 56, 56, 448, 616});
    walking_right.add_frame(joj::GLTextureRect{3, 8, 56, 56, 448, 616});
    walking_right.add_frame(joj::GLTextureRect{4, 8, 56, 56, 448, 616});
    walking_right.add_frame(joj::GLTextureRect{4, 8, 56, 56, 448, 616});
    walking_right.add_frame(joj::GLTextureRect{5, 8, 56, 56, 448, 616});
    walking_right.add_frame(joj::GLTextureRect{5, 8, 56, 56, 448, 616});
    walking_right.add_frame(joj::GLTextureRect{6, 8, 56, 56, 448, 616});
    walking_right.add_frame(joj::GLTextureRect{6, 8, 56, 56, 448, 616});
    walking_right.add_frame(joj::GLTextureRect{7, 8, 56, 56, 448, 616});
    walking_right.add_frame(joj::GLTextureRect{7, 8, 56, 56, 448, 616});

    joj::GLAnimation2D walking_left;
    walking_left.texture = &t2;
    walking_left.add_frame(joj::GLTextureRect{-1, +8, 56, 56, -448, +616});
    walking_left.add_frame(joj::GLTextureRect{-1, +8, 56, 56, -448, +616});
    walking_left.add_frame(joj::GLTextureRect{-2, +8, 56, 56, -448, +616});
    walking_left.add_frame(joj::GLTextureRect{-2, +8, 56, 56, -448, +616});
    walking_left.add_frame(joj::GLTextureRect{-3, +8, 56, 56, -448, +616});
    walking_left.add_frame(joj::GLTextureRect{-3, +8, 56, 56, -448, +616});
    walking_left.add_frame(joj::GLTextureRect{-4, +8, 56, 56, -448, +616});
    walking_left.add_frame(joj::GLTextureRect{-4, +8, 56, 56, -448, +616});
    walking_left.add_frame(joj::GLTextureRect{-5, +8, 56, 56, -448, +616});
    walking_left.add_frame(joj::GLTextureRect{-5, +8, 56, 56, -448, +616});
    walking_left.add_frame(joj::GLTextureRect{-6, +8, 56, 56, -448, +616});
    walking_left.add_frame(joj::GLTextureRect{-6, +8, 56, 56, -448, +616});
    walking_left.add_frame(joj::GLTextureRect{-7, +8, 56, 56, -448, +616});
    walking_left.add_frame(joj::GLTextureRect{-7, +8, 56, 56, -448, +616});
    walking_left.add_frame(joj::GLTextureRect{-8, +8, 56, 56, -448, +616});
    walking_left.add_frame(joj::GLTextureRect{-8, +8, 56, 56, -448, +616});

    joj::GLAnimation2D jumping_right;
    jumping_right.texture = &t2;
    jumping_right.add_frame(joj::GLTextureRect{0, 7, 56, 56, 448, 616});
    jumping_right.add_frame(joj::GLTextureRect{1, 7, 56, 56, 448, 616});
    jumping_right.add_frame(joj::GLTextureRect{2, 7, 56, 56, 448, 616});
    jumping_right.add_frame(joj::GLTextureRect{3, 7, 56, 56, 448, 616});
    jumping_right.add_frame(joj::GLTextureRect{4, 7, 56, 56, 448, 616});
    jumping_right.add_frame(joj::GLTextureRect{5, 7, 56, 56, 448, 616});
    jumping_right.add_frame(joj::GLTextureRect{6, 7, 56, 56, 448, 616});
    jumping_right.add_frame(joj::GLTextureRect{7, 7, 56, 56, 448, 616});
    jumping_right.add_frame(joj::GLTextureRect{0, 6, 56, 56, 448, 616});
    jumping_right.add_frame(joj::GLTextureRect{1, 6, 56, 56, 448, 616});
    jumping_right.add_frame(joj::GLTextureRect{2, 6, 56, 56, 448, 616});
    jumping_right.add_frame(joj::GLTextureRect{3, 6, 56, 56, 448, 616});
    jumping_right.add_frame(joj::GLTextureRect{4, 6, 56, 56, 448, 616});
    jumping_right.add_frame(joj::GLTextureRect{5, 6, 56, 56, 448, 616});
    jumping_right.add_frame(joj::GLTextureRect{6, 6, 56, 56, 448, 616});
    jumping_right.add_frame(joj::GLTextureRect{7, 6, 56, 56, 448, 616});

    joj::GLAnimation2D jumping_left;
    jumping_left.texture = &t2;
    jumping_left.add_frame(joj::GLTextureRect{-0, 7, 56, 56, -448, 616});
    jumping_left.add_frame(joj::GLTextureRect{-1, 7, 56, 56, -448, 616});
    jumping_left.add_frame(joj::GLTextureRect{-2, 7, 56, 56, -448, 616});
    jumping_left.add_frame(joj::GLTextureRect{-3, 7, 56, 56, -448, 616});
    jumping_left.add_frame(joj::GLTextureRect{-4, 7, 56, 56, -448, 616});
    jumping_left.add_frame(joj::GLTextureRect{-5, 7, 56, 56, -448, 616});
    jumping_left.add_frame(joj::GLTextureRect{-6, 7, 56, 56, -448, 616});
    jumping_left.add_frame(joj::GLTextureRect{-7, 7, 56, 56, -448, 616});
    jumping_left.add_frame(joj::GLTextureRect{-0, 6, 56, 56, -448, 616});
    jumping_left.add_frame(joj::GLTextureRect{-1, 6, 56, 56, -448, 616});
    jumping_left.add_frame(joj::GLTextureRect{-2, 6, 56, 56, -448, 616});
    jumping_left.add_frame(joj::GLTextureRect{-3, 6, 56, 56, -448, 616});
    jumping_left.add_frame(joj::GLTextureRect{-4, 6, 56, 56, -448, 616});
    jumping_left.add_frame(joj::GLTextureRect{-5, 6, 56, 56, -448, 616});
    jumping_left.add_frame(joj::GLTextureRect{-6, 6, 56, 56, -448, 616});
    jumping_left.add_frame(joj::GLTextureRect{-7, 6, 56, 56, -448, 616});

    joj::GLAnimation2D attacking;
    attacking.texture = &t2;
    attacking.add_frame(joj::GLTextureRect{0, 9, 56, 56, 448, 616});
    attacking.add_frame(joj::GLTextureRect{1, 9, 56, 56, 448, 616});
    attacking.add_frame(joj::GLTextureRect{2, 9, 56, 56, 448, 616});
    attacking.add_frame(joj::GLTextureRect{3, 9, 56, 56, 448, 616});
    attacking.add_frame(joj::GLTextureRect{4, 9, 56, 56, 448, 616});
    attacking.add_frame(joj::GLTextureRect{5, 9, 56, 56, 448, 616});
    attacking.add_frame(joj::GLTextureRect{6, 9, 56, 56, 448, 616});
    attacking.add_frame(joj::GLTextureRect{7, 9, 56, 56, 448, 616});

    joj::GLAnimation2D idle;
    idle.texture = &t2;
    idle.add_frame(joj::GLTextureRect{0, 10, 56, 56, 448, 616});
    idle.add_frame(joj::GLTextureRect{0, 10, 56, 56, 448, 616});
    idle.add_frame(joj::GLTextureRect{1, 10, 56, 56, 448, 616});
    idle.add_frame(joj::GLTextureRect{1, 10, 56, 56, 448, 616});
    idle.add_frame(joj::GLTextureRect{2, 10, 56, 56, 448, 616});
    idle.add_frame(joj::GLTextureRect{2, 10, 56, 56, 448, 616});
    idle.add_frame(joj::GLTextureRect{3, 10, 56, 56, 448, 616});
    idle.add_frame(joj::GLTextureRect{3, 10, 56, 56, 448, 616});
    idle.add_frame(joj::GLTextureRect{4, 10, 56, 56, 448, 616});
    idle.add_frame(joj::GLTextureRect{4, 10, 56, 56, 448, 616});
    idle.add_frame(joj::GLTextureRect{5, 10, 56, 56, 448, 616});
    idle.add_frame(joj::GLTextureRect{5, 10, 56, 56, 448, 616});

    joj::GLTextureRect rect = idle.m_frames[0];
    shader.set_vec4("rect", rect);

    f32 delta_time = 0.0f;              // Time between current frame and last frame
    f32 last_frame = get_abs_time();
    f32 current_frame;

    f64 accumulated_time = 0.0f;

    u32 i = 0;

    joj::GLAnimation2D* current_anim = &idle;
    while (pm->is_running())
    {
        current_frame = get_abs_time();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        accumulated_time += delta_time;

        current_anim = &idle;
        pm->process_events();

        rect = idle.m_frames[i];

        if (pm->is_key_pressed(KEY_ESCAPE))
            pm->close_window();

        if (pm->is_key_down(KEY_D))
            current_anim = &walking_right;

        if (pm->is_key_down(KEY_A))
            current_anim = &walking_left;

        if (pm->is_key_down(KEY_Z))
            current_anim = &attacking;

        if (pm->is_key_down(KEY_SPACE))
            current_anim = &jumping_right;

        std::cout << "delta_time = " << delta_time << "\n";
        std::cout << "        accum_time = " << accumulated_time << "\n";

        if (accumulated_time > TARGET_FRAME_TIME)
        {
            i = (i + 1) % current_anim->m_frames.size();
            rect = current_anim->m_frames[i];
            shader.set_vec4("rect", rect);
            shader.set_float("time", delta_time);
            accumulated_time = 0.0f;
        }
        else
        {
            rect = current_anim->m_frames[i];
        }

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
