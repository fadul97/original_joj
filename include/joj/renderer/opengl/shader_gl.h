#ifndef JOJ_GL_SHADER_H
#define JOJ_GL_SHADER_H

#define JOJ_GL_DEFINE_EXTERN
#include "platform/context/opengl/joj_gl.h"

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "renderer/shader.h"

#include "texture_rect_gl.h"
#include "math/jmath.h"

namespace joj
{
    enum class ShaderType { VERTEX, FRAGMENT, PROGRAM };

    class JAPI GLShader : public Shader
    {
    public:
        GLShader();
        GLShader(const char* vertex_shader, const char* fragment_shader);
        ~GLShader() override;
        
        b8 compile_shaders(const char* vertex_shader, const char* fragment_shader);
        void use() const override;
        void set_int(const std::string& name, i32 value) const;
        void set_float(const std::string& name, f32 value) const;
        void set_vec3(const std::string& name, Vector3 v) const;
        void set_vec4(const std::string& name, const GLTextureRect& r) const;
        void set_vec4(const std::string& name, Vector4 v) const;
        void set_mat4(const std::string& name, const Matrix4& m) const;
        void destroy() override;

    private:
        u32 m_id;
        static b8 check_compile_errors(i32 shader, ShaderType shader_type);
    };

    inline void GLShader::use() const
    { glUseProgram(m_id); }

    inline void GLShader::set_int(const std::string& name, const i32 value) const
    { glUniform1i(glGetUniformLocation(m_id, name.c_str()), value); }

    inline void GLShader::set_float(const std::string& name, const f32 value) const
    { glUniform1f(glGetUniformLocation(m_id, name.c_str()), value); }

    inline void GLShader::set_vec3(const std::string& name, const Vector3 v) const
    { glUniform3f(glGetUniformLocation(m_id, name.c_str()), v.x, v.y, v.z); }

    inline void GLShader::set_vec4(const std::string& name, const GLTextureRect& r) const
    { glUniform4f(glGetUniformLocation(m_id, name.c_str()), r.m_x0, r.m_y0, r.m_x1, r.m_y1); }

    inline void GLShader::set_vec4(const std::string& name, const Vector4 v) const
    { glUniform4f(glGetUniformLocation(m_id, name.c_str()), v.x, v.y, v.z, v.w); }

    inline void GLShader::set_mat4(const std::string& name, const Matrix4& m) const
    { 
        glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, m.data);
    }
    inline void GLShader::destroy()
    { glDeleteProgram(m_id); }
}

#endif // JOJ_GL_SHADER_H
