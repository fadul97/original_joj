#include "renderer/opengl/shader_gl.h"

#include "logger.h"

joj::GLShader::GLShader()
{
    m_id = 0;
}

joj::GLShader::GLShader(const char* vertex_shader, const char* fragment_shader)
{
    b8 failed = false;

    // Vertex shader
    const u32 vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_shader, nullptr);
    glCompileShader(vertex);
    if (!check_compile_errors(vertex, ShaderType::VERTEX))
    {
        JERROR(ErrorCode::ERR_VSHADER, "Failed to compile vertex shader.");
        failed = true;
    }

    // Fragment Shader
    const u32 fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_shader, nullptr);
    glCompileShader(fragment);
    if (!check_compile_errors(fragment, ShaderType::FRAGMENT))
    {
        JERROR(ErrorCode::ERR_FSHADER, "Failed to compile fragment shader.");
        failed = true;
    }

    if (failed) {
        m_id = 0;
    } else {
        // Shader Program
        m_id = glCreateProgram();
        glAttachShader(m_id, vertex);
        glAttachShader(m_id, fragment);
        glLinkProgram(m_id);
        if (!check_compile_errors(m_id, ShaderType::PROGRAM))
        {
            // TODO: Use own logger and return value
            JERROR(ErrorCode::ERR_SHADER_PROGRAM, "Failed to compile shader program.");
            glDeleteProgram(m_id);
        }
    }

    // Delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

joj::GLShader::~GLShader()
{
    glDeleteProgram(m_id);
}

b8 joj::GLShader::compile_shaders(const char* vertex_shader, const char* fragment_shader)
{
    b8 failed = false;

    // Vertex shader
    const u32 vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_shader, nullptr);
    glCompileShader(vertex);
    if (!check_compile_errors(vertex, ShaderType::VERTEX))
    {
        JERROR(ErrorCode::ERR_VSHADER, "Failed to compile vertex shader.");
        failed = true;
    }

    // Fragment Shader
    const u32 fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_shader, nullptr);
    glCompileShader(fragment);
    if (!check_compile_errors(fragment, ShaderType::FRAGMENT))
    {
        JERROR(ErrorCode::ERR_FSHADER, "Failed to compile fragment shader.");
        failed = true;
    }

    if (failed) {
        m_id = 0;
        return false;
    }

    // Shader Program
    m_id = glCreateProgram();
    glAttachShader(m_id, vertex);
    glAttachShader(m_id, fragment);
    glLinkProgram(m_id);
    if (!check_compile_errors(m_id, ShaderType::PROGRAM))
    {
        // TODO: Use own logger and return value
        JERROR(ErrorCode::ERR_SHADER_PROGRAM, "Failed to compile shader program.");
        glDeleteProgram(m_id);
        return false;
    }

    // Delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return true;
}

b8 joj::GLShader::check_compile_errors(const i32 shader, const ShaderType shader_type)
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
            glGetShaderInfoLog(shader, 1024, nullptr, info_log);
            JERROR(ErrorCode::ERR_SHADER_PROGRAM, "ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n",
                shadertype_name, info_log);
            return false;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            // TODO: Use own logger and return value
            glGetProgramInfoLog(shader, 1024, nullptr, info_log);
            JERROR(ErrorCode::ERR_SHADER_PROGRAM, "ERROR::PROGRAM_LINKING_ERROR of type: %s\n%s\n",
                shadertype_name, info_log);
            return false;
        }
    }

    return true;
}
