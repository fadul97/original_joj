#ifndef JOJ_SHADER_LIBRARY_GL_H
#define JOJ_SHADER_LIBRARY_GL_H

namespace joj::GLShaderLibrary {
    inline const char* vs_simple_transform = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "uniform mat4 transform;\n"
        "uniform vec4 inColor;\n"
        "out vec4 vertColor;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = transform * vec4(aPos, 1.0);\n"
        "    vertColor = inColor;\n"
        "}\0";

    inline const char* fs_simple_color = "#version 330 core\n"
    "in vec4 vertColor;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vertColor;\n"
    "}\0";

    inline const char* fs_light_and_obj_color = "#version 330 core\n"
    "uniform vec3 objectColor;\n"
    "uniform vec3 lightColor;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(lightColor * objectColor, 1.0);\n"
    "}\0";

    inline const char* vs_pos_and_normal = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aNormal;\n"
        "out vec3 FragPos;\n"
        "out vec3 Normal;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 proj;\n"
        "void main()\n"
        "{\n"
        "    FragPos = vec3(model * vec4(aPos, 1.0));\n"
        "    Normal = aNormal;\n"
        "\n"
        "    gl_Position = proj * view * vec4(FragPos, 1.0);\n"
        "}\0";

    inline const char* fs_light_and_normal = "#version 330 core\n"
        "in vec3 Normal;\n"
        "in vec3 FragPos;\n"
        "\n"
        "out vec4 FragColor;\n"
        "\n"
        "uniform vec3 lightPos; \n"
        "uniform vec3 lightColor;\n"
        "uniform vec3 objectColor;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    // ambient\n"
        "    float ambientStrength = 0.1;\n"
        "    vec3 ambient = ambientStrength * lightColor;\n"
        "\n"
        "    // diffuse\n"
        "    vec3 norm = normalize(Normal);\n"
        "    vec3 lightDir = normalize(lightPos - FragPos);\n"
        "    float diff = max(dot(norm, lightDir), 0.0);\n"
        "    vec3 diffuse = diff * lightColor;\n"
        "\n"
        "    vec3 result = (ambient + diffuse) * objectColor;\n"
        "    FragColor = vec4(result, 1.0);\n"
        "}\0";
};

#endif // JOJ_SHADER_LIBRARY_GL_H