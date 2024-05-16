#include <iostream>
#include <logger.h>
#include <renderer/opengl/shader_gl.h>

#include "joj/platform/win32/window_win32.h"
#include "joj/platform/context/opengl/win32/context_gl.h"
#include "joj/renderer/opengl/renderer_gl.h"

b8 process_events();

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main()
{
    joj::Win32Window window{};
    if (window.create(800, 600, "Refactored window", joj::WindowMode::Windowed) < joj::ErrorCode::OK) {
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

    const joj::GLShader shader{vertexShaderSource, fragmentShaderSource};

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    constexpr  float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };
    constexpr unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    b8 running = true;
    while (running) {
        if (!process_events()) {
            running = false;
        }

        renderer.clear(0.0f, 0.5f, 0.31f, 1.0f);

        // draw our first triangle
        shader.use();
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        window.swap_buffers();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

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