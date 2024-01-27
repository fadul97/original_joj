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
unsigned int shaderProgram = 0;
unsigned int VAO = 0;

int main()
{
#if JPLATFORM_LINUX
    std::cout << "LINUX!\n";
#elif JPLATFORM_WINDOWS
    std::cout << "WINDOWS!\n";
#endif

    auto pm = new joj::X11PlatformManager();
    pm->init(800, 600);
    pm->create_window();

    auto renderer = new joj::GLRenderer();
    // renderer->init(pm->get_window());


    joj::Vector3 v{};
    std::cout << "Vector3 = " << v.to_string() << "\n";

    joj::Vector4 v4{};
    std::cout << "Vector4 = " << v4.to_string() << "\n";

    joj::Matrix4 m{};
    std::cout << "Matrix4:\n" << m.to_string() << "\n";

    f32 r = joj::to_radians(45.0f);
    std::cout << "r = " << r << "\n";

    std::cout << "Creating shader.\n";
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::cout << "vertexShader = " << vertexShader << "\n";
    std::cout << "Finished creating shader.\n";
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    std::cout << "Compiled shaders!\n";
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    std::cout << "Success = " << success << "\n";
    std::cout << "vertexShader = " << vertexShader << "\n";
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    std::cout << "Success = " << success << "\n";
    std::cout << "fragmentShader = " << fragmentShader << "\n";
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
            -0.5f, -0.5f, 0.0f, // left
            0.5f, -0.5f, 0.0f, // right
            0.0f,  0.5f, 0.0f  // top
    };

    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);








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
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);

        pm->swap_buffers();
    }

    pm->shutdown();
    delete pm;
    
    std::cout << "Hello, Joj!\n";
    joj_print();

    u8 a = 0;
    std::cout << "a = " << a << "\n";

    u32 b = 10;
    std::cout << "b = " << b << "\n";

    i32 c = -10;
    std::cout << "c = " << c << "\n";
    return 0;
}
