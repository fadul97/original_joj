#ifndef JOJ_VECTOR4_H
#define JOJ_VECTOR4_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_LINUX

#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

namespace joj
{
    class JAPI Vector4
    {
    public:
        Vector4()
            : x(1.0f), y(1.0f), z(1.0f), w(1.0f)
        {
        }

        Vector4(f32 n)
            : x(n), y(n), z(n), w(n)
        {
        }

        Vector4(f32 x, f32 y, f32 z, f32 w)
            : x(x), y(y), z(z), w(w)
        {
        }

        ~Vector4() {}

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

        double length()
        { return sqrt((x * x) + (y * y) + (z * z) + (w * w)); }

        Vector4 operator-(const Vector4& v)
        {
            return Vector4 {
                x - v.x,
                y - v.y,
                z - v.z,
                w - v.w
            };
        }

        Vector4 operator/(const f32 k)
        {
            return Vector4 {
                x / k,
                y / k,
                z / k,
                w / k
            };
        }

        bool operator==(const Vector4& v)
        {
            for (i32 i = 0; i < 4; ++i)
            {
                if (elem[i] != v.elem[i])
                    return false;
            }

            return true;
        }

        std::string to_string() const
        {
            std::ostringstream oss;
            
            for (i32 i = 0; i < 4; ++i)
                oss << std::setw(10) << std::setprecision(4) << std::fixed << elem[i] << " ";

            return oss.str();
        }
    };
}

#endif // JPLATFORM_LINUX

#endif // JOJ_VECTOR4_H
