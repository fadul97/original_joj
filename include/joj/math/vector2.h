#ifndef JOJ_VECTOR2_H
#define JOJ_VECTOR2_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

namespace joj
{
    class JAPI Vector2
    {
    public:
        Vector2()
            : x(1.0f), y(1.0f)
        {
        }

        Vector2(f32 n)
            : x(n), y(n)
        {
        }

        Vector2(f32 x, f32 y)
            : x(x), y(y)
        {
        }

        ~Vector2() {}

        union
        {
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

        double length()
        { return sqrt((x * x) + (y * y)); }

        void normalize()
        { *this / this->length(); }

        Vector2 operator=(const Vector2& v)
        {
            x = v.x;
            y = v.y;

            return *this;
        }

        Vector2 operator-(const Vector2& v)
        {
            x -= v.x;
            y -= v.y;

            return *this;
        }

        Vector2 operator+(const Vector2& v)
        {
            Vector2 out;

            out.x = x + v.x;
            out.y = y + v.y;

            return out;
        }

        Vector2 operator/(const f32 k)
        {
            x /= k;
            y /= k;

            return *this;
        }

        Vector2 operator*(const f32 k)
        {
            x *= k;
            y *= k;

            return *this;
        }

        bool operator==(const Vector2& v)
        {
            for (i32 i = 0; i < 2; ++i)
            {
                if (elem[i] != v.elem[i])
                    return false;
            }

            return true;
        }

        std::string to_string() const
        {
            std::ostringstream oss;

            for (i32 i = 0; i < 2; ++i)
                oss << std::setw(10) << std::setprecision(4) << std::fixed << elem[i] << " ";

            return oss.str();
        }
    };

}

#endif // JOJ_VECTOR2_H
