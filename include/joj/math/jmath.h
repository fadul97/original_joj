#ifndef JOJ_MATH_H
#define JOJ_MATH_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS
#include <DirectXMath.h>
#define J_PI DirectX::XM_PI

#else

#include "vector3.h"
#include "vector4.h"
#include "matrix4.h"

#define J_PI 3.14159265358979323846f
#define JDEG2RAD J_PI / 180.0f

namespace joj
{
    inline f64 vec3_length(const Vector3 v)
    { return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z)); }

    inline f64 vec4_length(const Vector4 v)
    { return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z) + (v.w * v.w)); }

    inline Vector3 normalize(Vector3 v)
    { return v / vec3_length(v); }

    inline Vector4 normalize(Vector4 v)
    { return v / vec4_length(v); }

    inline Vector3 vec3_cross_product(Vector3 a, Vector3 b)
    {
        return Vector3 {
            a.y* b.z - a.z * b.y,
            a.z* b.x - a.x * b.z,
            a.x* b.y - a.y * b.x
        };
    }

    inline f64 vec3_dot_product(Vector3 a, Vector3 b)
    { return (a.x * b.x) + (a.y * b.y) + (a.z * b.z); }

    inline f32 to_radians(f32 degree)
    { return degree * JDEG2RAD; }

    inline Matrix4 identity()
    {
        Matrix4 out = Matrix4{};

        out.data[0] = 1.0f;
        out.data[5] = 1.0f;
        out.data[10] = 1.0f;
        out.data[15] = 1.0f;

        return out;
    }

    inline Matrix4 translate(Matrix4 in, f32 x, f32 y, f32 z)
    {
        Matrix4 out = in;

        out.data[12] += x;
        out.data[13] += y;
        out.data[14] += z;
        out.data[15] = 1.0f;

        return out;
    }

    inline Matrix4 translate(Matrix4 in, const Vector3 v)
    {
        Matrix4 out = in;

        out.data[12] += v.x;
        out.data[13] += v.y;
        out.data[14] += v.z;
        out.data[15] = 1.0f;

        return out;
    }

    inline Matrix4 look_at_rh(Vector3 position, Vector3 target, Vector3 up)
    {
        Vector3 forward = normalize(target - position);
        Vector3 right = normalize(vec3_cross_product(forward, up));
        Vector3 u = vec3_cross_product(right, forward);

        Matrix4 out = Matrix4{};

        out.elem[0][0] = right.x;
        out.elem[0][1] = u.x;
        out.elem[0][2] = -forward.x;

        out.elem[1][0] = right.y;
        out.elem[1][1] = u.y;
        out.elem[1][2] = -forward.y;

        out.elem[2][0] = right.z;
        out.elem[2][1] = u.z;
        out.elem[2][2] = -forward.z;

        out.elem[3][0] = -vec3_dot_product(right, position);
        out.elem[3][1] = -vec3_dot_product(u, position);
        out.elem[3][2] = vec3_dot_product(forward, position);

        return out;
    }

    inline Matrix4 look_at_lh(Vector3 position, Vector3 target, Vector3 up)
    {
        Vector3 forward = normalize(target - position);
        Vector3 right = normalize(vec3_cross_product(up, forward));
        Vector3 u = vec3_cross_product(forward, right);

        Matrix4 out = Matrix4{};

        out.elem[0][0] = right.x;
        out.elem[0][1] = u.x;
        out.elem[0][2] = forward.x;

        out.elem[1][0] = right.y;
        out.elem[1][1] = u.y;
        out.elem[1][2] = forward.y;
        
        out.elem[2][0] = right.z;
        out.elem[2][1] = u.z;
        out.elem[2][2] = forward.z;
        
        out.elem[3][0] = -vec3_dot_product(right, position);
        out.elem[3][1] = -vec3_dot_product(u, position);
        out.elem[3][2] = -vec3_dot_product(forward, position);

        return out;
    }

    inline Matrix4 perspective_lh(f32 fov_radians, f32 aspect_ratio, f32 near_plane, f32 far_plane)
    {
        f32 half_tan_fov = tan(fov_radians * 0.5f);

        Matrix4 out = Matrix4{0.0f};

        out.data[0] = 1.0f / (aspect_ratio * half_tan_fov);
        out.data[5] = 1.0f / half_tan_fov;
        out.data[10] = (far_plane + near_plane) / (far_plane - near_plane);
        out.data[11] = 1.0f;
        out.data[14] = - (2.0f * far_plane * near_plane) / (far_plane - near_plane);

        return out;
    }

    inline Matrix4 perspective_rh(f32 fov_radians, f32 aspect_ratio, f32 near_plane, f32 far_plane)
    {
        f32 half_tan_fov = tan(fov_radians * 0.5f);

        Matrix4 out = Matrix4{0.0f};

        out.data[0] = 1.0f / (aspect_ratio * half_tan_fov);
        out.data[5] = 1.0f / half_tan_fov;
        out.data[10] = - (far_plane + near_plane) / (far_plane - near_plane);
        out.data[11] = -1.0f;
        out.data[14] = - (2.0f * far_plane * near_plane) / (far_plane - near_plane);

        return out;
    }
}

#endif // JPLATFORM_LINUX

#endif // JOJ_MATH_H
