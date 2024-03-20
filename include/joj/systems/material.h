#ifndef JOJ_LIGHT_H
#define JOJ_LIGHT_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <Windows.h>
#include "math/vector4.h"

namespace joj
{
    struct Material
    {
        Material() { ZeroMemory(this, sizeof(this)); }

        Vector4 ambient;
        Vector4 diffuse;
        Vector4 specular; // w = Specular power
        Vector4 reflect;
    };
} // namespace joj

#endif // JOJ_LIGHT_H