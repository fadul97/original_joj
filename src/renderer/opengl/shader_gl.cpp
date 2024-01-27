#include "renderer/opengl/shader_gl.h"

#include <fstream>
#include <sstream>
#include <iostream>

joj::GLShader::GLShader()
{
    id = 0;
}

joj::GLShader::GLShader(const char* vertex_path, const char* fragment_path)
{
    // Open Vertex Shader file
    std::ifstream vshader_file;
    vshader_file.open(vertex_path);
    if (vshader_file.fail())
    {
        // TODO: Use own logger
        std::cout << "ERROR: Failed to open vertex shader file.\n";
    }

    // Open Fragment Shader file
    std::ifstream fshader_file;
    fshader_file.open(fragment_path);
    if (fshader_file.fail())
    {
        // TODO: Use own logger
        std::cout << "ERROR: Failed to open fragment shader file.\n";
    }

    // Read file's buffer contents into streams
    std::stringstream vshader_stream, fshader_stream;
    vshader_stream << vshader_file.rdbuf();
    fshader_stream << fshader_file.rdbuf();

    // Close file handlers
    vshader_file.close();
    fshader_file.close();    

    // Convert stream into string
    std::string vertex_code = vshader_stream.str();
    std::string fragment_code = fshader_stream.str();

    const char* vshader_code = vertex_code.c_str();
    const char* fshader_code = fragment_code.c_str();

    // Vertex shader
    u32 vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vshader_code, NULL);
    glCompileShader(vertex);
    check_compile_errors(vertex, ShaderType::VERTEX);
    
    // Fragment Shader
    u32 fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fshader_code, NULL);
    glCompileShader(fragment);
    check_compile_errors(fragment, ShaderType::FRAGMENT);

    // Shader Program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    check_compile_errors(id, ShaderType::PROGRAM);

    // Delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

joj::GLShader::~GLShader()
{
}

b8 joj::GLShader::compile_shaders(const char* vertex_shader, const char* fragment_shader)
{
    // Vertex shader
    u32 vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_shader, NULL);
    glCompileShader(vertex);
    if (!check_compile_errors(vertex, ShaderType::VERTEX))
    {
        // TODO: Use own logger and return value
        std::cout << "ERROR: Failed to compile vertex shader.\n";
        return false;
    }
    
    // Fragment Shader
    u32 fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_shader, NULL);
    glCompileShader(fragment);
    if (!check_compile_errors(fragment, ShaderType::FRAGMENT))
    {
        // TODO: Use own logger and return value
        std::cout << "ERROR: Failed to compile fragment shader.\n";
        return false;
    }

    // Shader Program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    if (!check_compile_errors(id, ShaderType::PROGRAM))
    {
        // TODO: Use own logger and return value
        std::cout << "ERROR: Failed to compile shader program.\n";
        return false;
    }

    // Delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return true;
}

b8 joj::GLShader::check_compile_errors(i32 shader, ShaderType shader_type)
{
    std::string shadertype_name;
    switch (shader_type)
    {
    case ShaderType::VERTEX:
        shadertype_name = std::string{ "VERTEX" };
        break;

    case ShaderType::FRAGMENT:
        shadertype_name = std::string{ "FRAGMENT" };
        break;

    case ShaderType::PROGRAM:
        shadertype_name = std::string{ "PROGRAM" };
        break;
    }

    i32 success;
    char info_log[1024];
    
    if (shader_type != ShaderType::PROGRAM)
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            // TODO: Use own logger and return value
            glGetShaderInfoLog(shader, 1024, NULL, info_log);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << shadertype_name << "\n" << info_log << "\n -- --------------------------------------------------- -- " << std::endl;
            return false;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            // TODO: Use own logger and return value
            glGetProgramInfoLog(shader, 1024, NULL, info_log);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << shadertype_name << "\n" << info_log << "\n -- --------------------------------------------------- -- " << std::endl;
            return false;
        }
    }

    return true;
}
