#ifndef JOJ_LIGHT_H
#define JOJ_LIGHT_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <Windows.h>
#include "math/vector4.h"
#include "math/vector3.h"

namespace joj
{
    struct DirectionalLight
    {
        DirectionalLight() { ZeroMemory(this, sizeof(this)); }

        Vector4 ambient;
        Vector4 diffuse;
        Vector4 specular;
        Vector3 direction;
        f32 pad; // Pad the last float so we can array of lights if we want
    };

    struct PointLight
    {
        PointLight() { ZeroMemory(this, sizeof(this)); }

        Vector4 ambient;
        Vector4 diffuse;
        Vector4 specular;

        // Packed into 4D vector: (position, range)
        Vector3 position;
        f32 range;

        // Packed into 4D vector: (attenuation, pad)
        Vector3 attenuation;
        f32 pad; // Pad the last float so we can array of lights if we want
    };

    struct SpotLight
    {
        SpotLight() { ZeroMemory(this, sizeof(this)); }

        Vector4 ambient;
        Vector4 diffuse;
        Vector4 specular;

        // Packed into 4D vector: (position, range)
        Vector3 position;
        f32 range;

        // Packed into 4D vector: (direction, spot)
        Vector3 direction;
        f32 spot;

        // Packed into 4D vector: (attenuation, pad)
        Vector3 attenuation;
        f32 pad; // Pad the last float so we can array of lights if we want
    };
} // namespace joj

#endif // JOJ_LIGHT_H