#ifndef JOJ_SHADER_H
#define JOJ_SHADER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <string>
#include "math/matrix4.h"
#include "math/vector3.h"

namespace joj
{
    class JAPI Shader
    {
    public:  
        Shader();
        Shader(const char* vertex_path, const char* fragment_path);
        virtual ~Shader();

        virtual b8 compile_shaders(const char* vertex_shader, const char* fragment_shader) = 0;
        virtual void use() const = 0;
        virtual void set_vec3(const std::string name, const Vector3 v) const = 0;
        virtual void set_vec4(const std::string name, const Vector4 v) const = 0;
        virtual void set_mat4(const std::string name, const Matrix4 m) const = 0;
    };
}
#endif // JOJ_SHADER_H
