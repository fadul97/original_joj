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
                                       "   FragColor = vec4(1.0f, 0.5f, 1.0f, 1.0f);\n"
                                       "}\n\0";
unsigned int VAO = 0;

int main()
{
    auto pm = new joj::X11PlatformManager();
    pm->init(800, 600);
    pm->create_window();

    auto renderer = new joj::GLRenderer();

    joj::GLShader *shader = new joj::GLShader{};
    if (!shader->compile_shaders(vertexShaderSource, fragmentShaderSource))
        std::cout << "Failed to compile shaders.\n";


    // -----------------------------------------------------------------------------------------------------------------
    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
            -0.5f, -0.5f, 0.0f, // left
            0.5f, -0.5f, 0.0f,  // right
            0.0f,  0.5f, 0.0f   // top
    };

    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // -----------------------------------------------------------------------------------------------------------------

    
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

        // draw our first triangle
        shader->use();
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);

        pm->swap_buffers();
    }

    pm->shutdown();

    delete shader;
    delete renderer;
    delete pm;
    
    return 0;
}
