#ifndef JOJ_MATH_H
#define JOJ_MATH_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <cstring>
#include <cmath>
#include <iomanip>

#define J_PI 3.14159265358979323846f
#define JDEG2RAD (J_PI / 180.0f)

namespace joj {
    struct JAPI Vector2 {
        union {
            f32 elem[2];
            struct
            {
                union
                {
                    f32 x, s, u;
                };
                union
                {
                    f32 y, t, v;
                };
            };
        };
    };

    struct JAPI Vector3 {
        union
        {
            f32 elem[3];
            struct
            {
                union
                {
                    f32 x, r, s, u;
                };
                union
                {
                    f32 y, g, t, v;
                };
                union
                {
                    f32 z, b, p, w;
                };
            };
        };
    };

    struct JAPI Vector4 {
        union
        {
            f32 elem[4];
            struct
            {
                union
                {
                    f32 x, r, s;
                };
                union
                {
                    f32 y, g, t;
                };
                union
                {
                    f32 z, b, p;
                };
                union
                {
                    f32 w, a, q;
                };
            };
        };
    };

    struct JAPI Matrix4 {
        union {
            f32 elem[4][4];
            f32 data[16];
        };
    };
};

namespace joj {
    inline Vector3 operator-(const Vector3 v0, const Vector3 v1)
    {
        Vector3 out{};

        out.x = v0.x - v1.x;
        out.y = v0.y - v1.y;
        out.z = v0.z - v1.z;

        return out;
    }

    inline Vector3 operator+(const Vector3 v0, const Vector3 v1)
    {
        Vector3 out{};

        out.x = v0.x + v1.x;
        out.y = v0.y + v1.y;
        out.z = v0.z + v1.z;

        return out;
    }

    inline Vector3 operator/(const Vector3 v, const f32 k)
    {
        Vector3 out{};

        out.x = v.x / k;
        out.y = v.y / k;
        out.z = v.z / k;

        return out;
    }

    inline Vector3 operator*(const Vector3 v, const f32 k)
    {
        Vector3 out{};

        out.x = v.x * k;
        out.y = v.y * k;
        out.z = v.z * k;

        return out;
    }

    inline Matrix4 operator*(const Matrix4& m0, const Matrix4& m1) {
        Matrix4 result{};

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.elem[i][j] = 0;
                for (int k = 0; k < 4; ++k) {
                    result.elem[i][j] += m0.elem[i][k] * m1.elem[k][j];
                }
            }
        }

        return result;
    }
}

namespace joj::MathHelper {
    inline std::string vec2_to_str(const Vector2 v) {
        std::ostringstream oss;

        for (const f32 i : v.elem)
            oss << std::setw(10) << std::setprecision(4) << std::fixed << i << " ";

        return oss.str();
    }

    inline std::string vec3_to_str(const Vector3 v) {
        std::ostringstream oss;

        for (const f32 i : v.elem)
            oss << std::setw(10) << std::setprecision(4) << std::fixed << i << " ";

        return oss.str();
    }

    inline std::string vec4_to_str(const Vector4 v) {
        std::ostringstream oss;

        for (const f32 i : v.elem)
            oss << std::setw(10) << std::setprecision(4) << std::fixed << i << " ";

        return oss.str();
    }

    inline std::string mat4_to_str(const Matrix4& m) {
        std::ostringstream oss;

        for (const auto i : m.elem) {
            for (i32 j = 0; j < 4; ++j) {
                oss << std::setw(10) << std::setprecision(4) << std::fixed << i[j] << "  ";
            }
            oss << "\n";
        }

        std::string str = oss.str();
        return str;
    }

    inline f32 to_radians(const f32 degree)
    { return degree * JDEG2RAD; }

    inline Vector2 vec2_create(const f32 x, const f32 y) {
        return Vector2 {
            .x = x,
            .y = y
        };
    }

    inline Vector3 vec3_create(const f32 x, const f32 y, const f32 z) {
        return Vector3 {
            .x = x,
            .y = y,
            .z = z
        };
    }

    inline Vector4 vec4_create(const f32 x, const f32 y, const f32 z, const f32 w) {
        return Vector4 {
            .x = x,
            .y = y,
            .z = z,
            .w = w
        };
    }

    inline Matrix4 mat4_id() {
        Matrix4 out{};

        memset(out.data, 0, sizeof(out.data));

        out.elem[0][0] = 1.0f;
        out.elem[1][1] = 1.0f;
        out.elem[2][2] = 1.0f;
        out.elem[3][3] = 1.0f;

        return out;
    }

    inline Vector3 vec3_cpy(const Vector3 v) {
        return (Vector3) {
            .x = v.x,
            .y = v.y,
            .z = v.z
        };
    }

    inline Vector3 vec3_sub(const Vector3 v0, const Vector3 v1) {
        return (Vector3){v0.x - v1.x, v0.y - v1.y,
                      v0.z - v1.z};
    }

    inline f32 vec3_length_squared(const Vector3 v) {
        return v.x * v.x + v.y * v.y + v.z * v.z;
    }

    inline f32 vec3_length(const Vector3 v) {
        return static_cast<f32>(sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z)));
    }

    inline void vec3_normalize(Vector3* v) {
        const f32 length = vec3_length(*v);
        v->x /= length;
        v->y /= length;
        v->z /= length;
    }

    inline Vector3 vec3_cross_product(const Vector3 v0, const Vector3 v1)
    {
        return Vector3 {
            v0.y* v1.z - v0.z * v1.y,
            v0.z* v1.x - v0.x * v1.z,
            v0.x* v1.y - v0.y * v1.x
        };
    }

    inline f64 vec3_dot_product(const Vector3 v0, const Vector3 v1)
    { return (v0.x * v1.x) + (v0.y * v1.y) + (v0.z * v1.z); }

    inline Vector3 vec3_normalized(Vector3 v) {
        const f32 length = vec3_length(v);

        const Vector3 out {
            .x = v.x /= length,
            .y = v.y /= length,
            .z = v.z /= length,
        };

        // vec3_normalize(&v);
        return out;
    }

    inline Vector3 vec3_cross(const Vector3 v0, const Vector3 v1) {
        return (Vector3){
            v0.y * v1.z - v0.z * v1.y,
            v0.z * v1.x - v0.x * v1.z,
            v0.x * v1.y - v0.y * v1.x
        };
    }

    inline f32 vec3_dot(const Vector3 v0, const Vector3 v1) {
        return (v0.x * v1.x) + (v0.y * v1.y) + (v0.z * v1.z);
    }

    inline Vector3 vec3_add(const Vector3 v0, const Vector3 v1) {
        return (Vector3) {
            .x = v0.x + v1.x,
            .y = v0.y + v1.y,
            .z = v0.z + v1.z,
        };
    }

    inline Vector3 vec3_minus(const Vector3 v0, const Vector3 v1) {
        return (Vector3) {
            .x = v0.x - v1.x,
            .y = v0.y - v1.y,
            .z = v0.z - v1.z,
        };
    }

    inline Vector3 vec3_mul_by_scalar(const Vector3 v, const f32 k) {
        // Vector3 out{};
        // out.x = v.x * k;
        // out.y = v.y * k;
        // out.z = v.z * k;
        // return out;
        return (Vector3) {
            .x = v.x * k,
            .y = v.y * k,
            .z = v.z * k,
        };
    }

    inline Vector3 vec4_to_vec3(const Vector4 v) {
        return (Vector3){v.x, v.y, v.z};
    }

    inline Matrix4 look_at_rh(Vector3 position, Vector3 target, Vector3 up)
    {
        Vector3 forward = vec3_normalized(vec3_sub(target, position));
        Vector3 right = vec3_normalized(vec3_cross(forward, up));
        Vector3 u = vec3_cross(right, forward);

        Matrix4 out = mat4_id();

        out.elem[0][0] = right.x;
        out.elem[0][1] = u.x;
        out.elem[0][2] = -forward.x;

        out.elem[1][0] = right.y;
        out.elem[1][1] = u.y;
        out.elem[1][2] = -forward.y;

        out.elem[2][0] = right.z;
        out.elem[2][1] = u.z;
        out.elem[2][2] = -forward.z;

        out.elem[3][0] = -vec3_dot(right, position);
        out.elem[3][1] = -vec3_dot(u, position);
        out.elem[3][2] = vec3_dot(forward, position);

        return out;
    }

    inline Vector3 normalize(Vector3 v)
    { return v / vec3_length(v); }

    inline Matrix4 over_look_at_rh(Vector3 position, Vector3 target, Vector3 up)
    {
        Vector3 forward = normalize(target - position);
        Vector3 right = normalize(vec3_cross_product(forward, up));
        Vector3 u = vec3_cross_product(right, forward);

        Matrix4 out = mat4_id();

        out.elem[0][0] = right.x;
        out.elem[0][1] = u.x;
        out.elem[0][2] = -forward.x;

        out.elem[1][0] = right.y;
        out.elem[1][1] = u.y;
        out.elem[1][2] = -forward.y;

        out.elem[2][0] = right.z;
        out.elem[2][1] = u.z;
        out.elem[2][2] = -forward.z;

        out.elem[3][0] = -static_cast<f32>(vec3_dot_product(right, position));
        out.elem[3][1] = -static_cast<f32>(vec3_dot_product(u, position));
        out.elem[3][2] =  static_cast<f32>(vec3_dot_product(forward, position));

        return out;
    }

    inline Matrix4 look_at_lh(Vector3 position, Vector3 target, Vector3 up)
    {
        Vector3 forward = vec3_normalized(vec3_sub(position, target));
        Vector3 right = vec3_normalized(vec3_cross(up, forward));
        Vector3 u = vec3_cross(forward, right);

        Matrix4 out = mat4_id();

        out.elem[0][0] = right.x;
        out.elem[0][1] = u.x;
        out.elem[0][2] = forward.x;

        out.elem[1][0] = right.y;
        out.elem[1][1] = u.y;
        out.elem[1][2] = forward.y;

        out.elem[2][0] = right.z;
        out.elem[2][1] = u.z;
        out.elem[2][2] = forward.z;

        out.elem[3][0] = -vec3_dot(right, position);
        out.elem[3][1] = -vec3_dot(u, position);
        out.elem[3][2] = -vec3_dot(forward, position);

        return out;
    }

    inline Matrix4 over_look_at_lh(Vector3 position, Vector3 target, Vector3 up)
    {
        Vector3 forward = normalize(target - position);
        Vector3 right = normalize(vec3_cross_product(up, forward));
        Vector3 u = vec3_cross_product(forward, right);

        Matrix4 out = mat4_id();

        out.elem[0][0] = right.x;
        out.elem[0][1] = u.x;
        out.elem[0][2] = forward.x;

        out.elem[1][0] = right.y;
        out.elem[1][1] = u.y;
        out.elem[1][2] = forward.y;

        out.elem[2][0] = right.z;
        out.elem[2][1] = u.z;
        out.elem[2][2] = forward.z;

        out.elem[3][0] = -static_cast<f32>(vec3_dot_product(right, position));
        out.elem[3][1] = -static_cast<f32>(vec3_dot_product(u, position));
        out.elem[3][2] = -static_cast<f32>(vec3_dot_product(forward, position));

        return out;
    }

    inline Matrix4 perspective_lh(const f32 fov_radians, const f32 aspect_ratio, const f32 near_plane, const f32 far_plane)
    {
        const f32 half_tan_fov = std::tan(fov_radians * 0.5f);

        Matrix4 out = mat4_id();

        out.data[0] = 1.0f / (aspect_ratio * half_tan_fov);
        out.data[5] = 1.0f / half_tan_fov;
        out.data[10] = (far_plane + near_plane) / (far_plane - near_plane);
        out.data[11] = -1.0f;
        out.data[14] = - (2.0f * far_plane * near_plane) / (far_plane - near_plane);

        return out;
    }

    inline Matrix4 mat4_mul(const Matrix4& m0, const Matrix4& m1) {
        // Matrix4 out = mat4_id();
        //
        // for (u8 i = 0; i < 4; ++i)
        // {
        //     for (u8 j = 0; j < 4; ++j)
        //     {
        //         out.elem[i][j] = m0.elem[i][0] * m1.elem[0][j] +
        //                          m0.elem[i][1] * m1.elem[1][j] +
        //                          m0.elem[i][2] * m1.elem[2][j] +
        //                          m0.elem[i][3] * m1.elem[3][j];
        //     }
        // }
        //
        // return out;

        Matrix4 out = mat4_id();

        const f32 *m1_ptr = m0.data;
        const f32 *m2_ptr = m1.data;
        f32 *dst_ptr = out.data;

        for (i32 i = 0; i < 4; ++i) {
            for (i32 j = 0; j < 4; ++j) {
                *dst_ptr = m1_ptr[0] * m2_ptr[0 + j] + m1_ptr[1] * m2_ptr[4 + j] +
                           m1_ptr[2] * m2_ptr[8 + j] + m1_ptr[3] * m2_ptr[12 + j];
                dst_ptr++;
            }
            m1_ptr += 4;
        }
        return out;
    }

    inline Matrix4 mat4_translation(const Vector3 position) {
        Matrix4 out_matrix = mat4_id();
        out_matrix.data[12] = position.x;
        out_matrix.data[13] = position.y;
        out_matrix.data[14] = position.z;
        return out_matrix;
    }

    inline Matrix4 mat4_translate(Matrix4& m, const Vector3 v) {
        m.data[12] += v.x;
        m.data[13] += v.y;
        m.data[14] += v.z;
        return m;
    }

    inline Matrix4 mat4_translatef(Matrix4& m, const f32 x, const f32 y, const f32 z) {
        m.data[12] += x;
        m.data[13] += y;
        m.data[14] += z;
        return m;
    }
}

#endif // JOJ_MATH_H
