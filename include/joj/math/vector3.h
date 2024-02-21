#ifndef JOJ_VECTOR3_H
#define JOJ_VECTOR3_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

// #if JPLATFORM_LINUX

#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

namespace joj
{
    class JAPI Vector3
    {
    public:
        Vector3()
            : x(1.0f), y(1.0f), z(1.0f)
        {
        }

        Vector3(f32 n)
            : x(n), y(n), z(n)
        {
        }

        Vector3(f32 x, f32 y, f32 z)
            : x(x), y(y), z(z)
        {
        }

        ~Vector3() {}

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

        double length()
        { return sqrt((x * x) + (y * y) + (z * z)); }

        void normalize()
        { *this / this->length(); }

        Vector3 operator=(const Vector3& v)
        {
            x = v.x;
            y = v.y;
            z = v.z;

            return *this;
        }

        Vector3 operator-(const Vector3& v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;

            return *this;
        }

        Vector3 operator+(const Vector3& v)
        {
            Vector3 out;

            out.x = x + v.x;
            out.y = y + v.y;
            out.z = z + v.z;

            return out;
        }

        Vector3 operator/(const f32 k)
        {
            x /= k;
            y /= k;
            z /= k;

            return *this;
        }

        Vector3 operator*(const f32 k)
        {
            x *= k;
            y *= k;
            z *= k;

            return *this;
        }

        // vector3 operator*(const f32 k, const vector3& v)
        // {
        //     x *= k;
        //     y *= k;
        //     z *= k;

        //     return *this;
        // }

        bool operator==(const Vector3& v)
        {
            for (i32 i = 0; i < 3; ++i)
            {
                if (elem[i] != v.elem[i])
                    return false;
            }

            return true;
        }

        std::string to_string() const
        {
            std::ostringstream oss;
            
            for (i32 i = 0; i < 3; ++i)
                oss << std::setw(10) << std::setprecision(4) << std::fixed << elem[i] << " ";

            return oss.str();
        }
    };

}

// #endif // JPLATFORM_LINUX

#endif // JOJ_VECTOR3_H
