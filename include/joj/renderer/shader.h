#ifndef JOJ_SHADER_H
#define JOJ_SHADER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <string>
#include "math/jmath.h"

namespace joj
{
    class JAPI Shader
    {
    public:  
        Shader();
        virtual ~Shader();

        virtual void use() const = 0;
        virtual void destroy() = 0;
    };
}
#endif // JOJ_SHADER_H
