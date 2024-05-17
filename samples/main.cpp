#include <iostream>
#include <logger.h>
#include <platform/win32/input_win32.h>
#include <renderer/opengl/shader_gl.h>
#include "joj/platform/win32/window_win32.h"
#include "joj/platform/context/opengl/win32/context_gl.h"
#include "joj/renderer/opengl/renderer_gl.h"
#include "joj/renderer/opengl/shader_library_gl.h"
#include "joj/resources/vertex.h"
#include "joj/resources/mesh.h"
#include "joj/systems/camera.h"

#define WIDTH 800
#define HEIGHT 600

b8 process_events();

int main()
{
    joj::Win32Window window{};
    if (window.create(WIDTH, HEIGHT, "Refactored window", joj::WindowMode::Windowed) < joj::ErrorCode::OK) {
        return -1;
    }

    u16 width = 0;
    u16 height = 0;
    window.get_window_size(width, height);
    printf("Window size: %dx%d\n", width, height);

    window.get_client_size(width, height);
    printf("Client size: %dx%d\n", width, height);

    joj::Win32GLContext context{};
    if (context.create(window.get_window_config()) < joj::ErrorCode::OK) {
        window.destroy();
        return -2;
    }

    if (context.make_current(window.get_window_config()) < joj::ErrorCode::OK) {
        window.destroy();
        return -3;
    }

    joj::GLRenderer renderer{};
    if (renderer.init(window.get_window_config()) < joj::ErrorCode::OK) {
        JERROR(joj::ErrorCode::ERR_RENDERER, "Failed to initialize GLRenderer");
        return -4;
    }

    const joj::GLShader shader{
        joj::GLShaderLibrary::vs_simple_transform,
        joj::GLShaderLibrary::fs_simple_color
    };

    const joj::Vertex quad_vertices[] =
    {
        { joj::Vector3{-0.5f, -0.5f, -0.0f},  joj::Vector3{0.0f,  0.0f, -1.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { joj::Vector3{ 0.5f, -0.5f, -0.5f},  joj::Vector3{0.0f,  0.0f, -1.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { joj::Vector3{ 0.5f,  0.5f, -0.5f},  joj::Vector3{0.0f,  0.0f, -1.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { joj::Vector3{ 0.5f,  0.5f, -0.5f},  joj::Vector3{0.0f,  0.0f, -1.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { joj::Vector3{-0.5f,  0.5f, -0.5f},  joj::Vector3{0.0f,  0.0f, -1.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { joj::Vector3{-0.5f, -0.5f, -0.5f},  joj::Vector3{0.0f,  0.0f, -1.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },

        { joj::Vector3{-0.5f, -0.5f,  0.5f},  joj::Vector3{0.0f,  0.0f,  1.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { joj::Vector3{ 0.5f, -0.5f,  0.5f},  joj::Vector3{0.0f,  0.0f,  1.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { joj::Vector3{ 0.5f,  0.5f,  0.5f},  joj::Vector3{0.0f,  0.0f,  1.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { joj::Vector3{ 0.5f,  0.5f,  0.5f},  joj::Vector3{0.0f,  0.0f,  1.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { joj::Vector3{-0.5f,  0.5f,  0.5f},  joj::Vector3{0.0f,  0.0f,  1.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { joj::Vector3{-0.5f, -0.5f,  0.5f},  joj::Vector3{0.0f,  0.0f,  1.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },

        { joj::Vector3{-0.5f,  0.5f,  0.5f},  joj::Vector3{-1.0f,  0.0f,  0.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { joj::Vector3{-0.5f,  0.5f, -0.5f},  joj::Vector3{-1.0f,  0.0f,  0.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { joj::Vector3{-0.5f, -0.5f, -0.5f},  joj::Vector3{-1.0f,  0.0f,  0.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { joj::Vector3{-0.5f, -0.5f, -0.5f},  joj::Vector3{-1.0f,  0.0f,  0.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { joj::Vector3{-0.5f, -0.5f,  0.5f},  joj::Vector3{-1.0f,  0.0f,  0.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { joj::Vector3{-0.5f,  0.5f,  0.5f},  joj::Vector3{-1.0f,  0.0f,  0.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },

        { joj::Vector3{0.5f,  0.5f,  0.5f},  joj::Vector3{1.0f,  0.0f,  0.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { joj::Vector3{0.5f,  0.5f, -0.5f},  joj::Vector3{1.0f,  0.0f,  0.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { joj::Vector3{0.5f, -0.5f, -0.5f},  joj::Vector3{1.0f,  0.0f,  0.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { joj::Vector3{0.5f, -0.5f, -0.5f},  joj::Vector3{1.0f,  0.0f,  0.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { joj::Vector3{0.5f, -0.5f,  0.5f},  joj::Vector3{1.0f,  0.0f,  0.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { joj::Vector3{0.5f,  0.5f,  0.5f},  joj::Vector3{1.0f,  0.0f,  0.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },

        { joj::Vector3{-0.5f, -0.5f, -0.5f},  joj::Vector3{0.0f, -1.0f,  0.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { joj::Vector3{ 0.5f, -0.5f, -0.5f},  joj::Vector3{0.0f, -1.0f,  0.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { joj::Vector3{ 0.5f, -0.5f,  0.5f},  joj::Vector3{0.0f, -1.0f,  0.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { joj::Vector3{ 0.5f, -0.5f,  0.5f},  joj::Vector3{0.0f, -1.0f,  0.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { joj::Vector3{-0.5f, -0.5f,  0.5f},  joj::Vector3{0.0f, -1.0f,  0.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { joj::Vector3{-0.5f, -0.5f, -0.5f},  joj::Vector3{0.0f, -1.0f,  0.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },

        { joj::Vector3{-0.5f,  0.5f, -0.5f},  joj::Vector3{0.0f,  1.0f,  0.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { joj::Vector3{ 0.5f,  0.5f, -0.5f},  joj::Vector3{0.0f,  1.0f,  0.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { joj::Vector3{ 0.5f,  0.5f,  0.5f},  joj::Vector3{0.0f,  1.0f,  0.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { joj::Vector3{ 0.5f,  0.5f,  0.5f},  joj::Vector3{0.0f,  1.0f,  0.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { joj::Vector3{-0.5f,  0.5f,  0.5f},  joj::Vector3{0.0f,  1.0f,  0.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} },
        { joj::Vector3{-0.5f,  0.5f, -0.5f},  joj::Vector3{0.0f,  1.0f,  0.0f}, joj::Vector4{1.0f, 1.0f, 0.0f, 1.0f} }
    };

    joj::Mesh quad{};
    // Add vertices to mesh
    for (const joj::Vertex& v : quad_vertices)
        quad.vertices.push_back(v);

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, quad.get_vertex_count() * sizeof(joj::Vertex), quad.get_vertex_data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(joj::Vertex), static_cast<void*>(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(joj::Vertex), (void*)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    joj::Matrix4 world = joj::MathHelper::mat4_id();

    joj::Camera camera{};
    camera.m_position = joj::MathHelper::vec3_create(0, 0, 10);
    // camera.m_target = joj::MathHelper::vec3_create(0, 0, 0);

    const joj::Matrix4 proj = joj::MathHelper::perspective_lh(
        joj::MathHelper::to_radians(45),
        static_cast<f32>(WIDTH) / HEIGHT,
        0.1f, 100.0f);

    joj::Matrix4 wvp = joj::MathHelper::mat4_mul(world, camera.get_view_mat());
    wvp = joj::MathHelper::mat4_mul(wvp, proj);

    joj::Win32Input input{};

    constexpr joj::Vector4 color{
        .x = 1.0f,
        .y = 0.5f,
        .z = 0.31f,
        .w = 1.0f
    };

    constexpr f32 dt = 0.05f;

    b8 running = true;
    while (running) {
        if (!process_events()) {
            running = false;
        }

        if (joj::Win32Input::is_key_down(joj::KEY_UP)) {
            camera.process_keyboard(joj::CameraMovement::FORWARD, dt);
        }

        if (joj::Win32Input::is_key_down(joj::KEY_DOWN)) {
            camera.process_keyboard(joj::CameraMovement::BACKWARD, dt);
        }

        if (joj::Win32Input::is_key_down(joj::KEY_LEFT)) {
            camera.process_keyboard(joj::CameraMovement::LEFT, dt);
        }

        if (joj::Win32Input::is_key_down(joj::KEY_RIGHT)) {
            camera.process_keyboard(joj::CameraMovement::RIGHT, dt);
        }

        renderer.clear(0.23f, 0.23f, 0.23f, 1.0f);

        if (joj::Win32Input::is_key_pressed('W')) {
            world = joj::MathHelper::mat4_translatef(world, 0.0f, 0.0f, -3.0f);
        }

        if (joj::Win32Input::is_key_pressed('S')) {
            world = joj::MathHelper::mat4_translatef(world, 0.0f, 0.0f, 3.0f);
        }

        if (joj::Win32Input::is_key_pressed('D')) {
            world = joj::MathHelper::mat4_translatef(world, 1.0f, 0.0f, 0.0f);
        }

        if (joj::Win32Input::is_key_pressed('A')) {
            world = joj::MathHelper::mat4_translatef(world, -1.0f, 0.0f, 0.0f);
        }

        wvp = joj::MathHelper::mat4_mul(world, camera.get_view_mat());
        wvp = joj::MathHelper::mat4_mul(wvp, proj);

        // draw our first triangle
        shader.use();
        shader.set_vec4("inColor", color);
        shader.set_mat4("transform", wvp);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, quad.get_vertex_count());

        window.swap_buffers();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    renderer.shutdown();
    context.destroy();
    window.destroy();

    printf("Hello, Refactoring!\n");
    return 0;
}

b8 process_events() {
    MSG msg{};

    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (msg.message == WM_QUIT || msg.message == WM_DESTROY)
            return false;
    }

    return true;
}