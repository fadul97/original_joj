#ifndef JOJ_GL_SHADER_H
#define JOJ_GL_SHADER_H

#define JOJ_GL_DEFINE_EXTERN
#include "platform/context/opengl/joj_gl.h"

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "renderer/shader.h"

#include "texture_rect_gl.h"
#include "math/vector3.h"
#include "math/vector4.h"
#include "math/matrix4.h"

namespace joj
{
    enum class ShaderType { VERTEX, FRAGMENT, PROGRAM };

    class JAPI GLShader : public Shader
    {
    public:
        GLShader();
        GLShader(const char* vertex_path, const char* fragment_path);
        ~GLShader();
        
        b8 compile_shaders(const char* vertex_shader, const char* fragment_shader);
        void use() const override;
        void set_int(const std::string name, const i32 value) const;
        void set_float(const std::string name, const f32 value) const;
        void set_vec3(const std::string name, const Vector3 v) const;
        void set_vec4(const std::string name, const GLTextureRect r) const;
        void set_vec4(const std::string name, const Vector4 v) const;
        void set_mat4(const std::string name, const Matrix4 m) const;
        void destroy() override;

    private:
        u32 id;
        b8 check_compile_errors(i32 shader, ShaderType shader_type);
    };

    inline void GLShader::use() const
    { glUseProgram(id); }

    inline void GLShader::set_int(const std::string name, const i32 value) const
    { glUniform1i(glGetUniformLocation(id, name.c_str()), value); }

    inline void GLShader::set_float(const std::string name, const f32 value) const
    { glUniform1f(glGetUniformLocation(id, name.c_str()), value); }

    inline void GLShader::set_vec3(const std::string name, const Vector3 v) const
    { glUniform3f(glGetUniformLocation(id, name.c_str()), v.x, v.y, v.z); }

    inline void GLShader::set_vec4(const std::string name, const GLTextureRect r) const
    { glUniform4f(glGetUniformLocation(id, name.c_str()), r.m_x0, r.m_y0, r.m_x1, r.m_y1); }

    inline void GLShader::set_vec4(const std::string name, const Vector4 v) const
    { glUniform4f(glGetUniformLocation(id, name.c_str()), v.x, v.y, v.z, v.w); }

#if JPLATFORM_WINDOWS
    inline void GLShader::set_mat4(const std::string name, const Matrix4 m) const
    { 
        // Convert DirectX::XMMATRIX to a compatible format
        float mat_array[16];
        memcpy(mat_array, &m, sizeof(float) * 16);
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, mat_array);
    }
#else
    inline void GLShader::set_mat4(const std::string name, const Matrix4 m) const
    { glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, m.data); }
#endif

    inline void GLShader::destroy()
    { glDeleteProgram(id); }
}

#endif // JOJ_GL_SHADER_H
