#ifndef JOJ_MATRIX_H
#define JOJ_MATRIX_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_LINUX

#include <string>
#include <sstream>
#include <iomanip>
#include "vector4.h"

namespace joj
{
    class JAPI Matrix4
    {
    public:
        Matrix4()
        {
            for (i32 i = 0; i < 16; i++)
                data[i] = 0;

            data[0] = 1.0f;
            data[5] = 1.0f;
            data[10] = 1.0f;
            data[15] = 1.0f;
        }

        Matrix4(f32 ar[16])
        {
            for (i32 i = 0; i < 16; i++)
                data[i] = ar[i];
        }

        Matrix4(f32 n){
            for (i32 i = 0; i < 16; i++)
                data[i] = n;
        }

        ~Matrix4(){}

        union
        {
            f32 elem[4][4];
            f32 data[16];
        };

        std::string to_string() const
        {
            std::string str;
            std::ostringstream oss;

            for (i32 i = 0; i < 4; ++i) {
                for (i32 j = 0; j < 4; ++j) {
                    oss << std::setw(10) << std::setprecision(4) << std::fixed << elem[i][j] << "  ";
                }
                oss << "\n";
            }

            str = oss.str();
            return str;
        }

        Matrix4 operator=(const Matrix4& m)
        {
            for(int i = 0; i < 16; ++i)
                data[i] = m.data[i];

            return *this;
        }

        Matrix4 operator*(const Matrix4& m)
        {
            Matrix4 out;

            for (u8 i = 0; i < 4; ++i)
            {
                for (u8 j = 0; j < 4; ++j)
                {
                    out.elem[i][j] = elem[i][0] * m.elem[0][j] +
                                     elem[i][1] * m.elem[1][j] +
                                     elem[i][2] * m.elem[2][j] +
                                     elem[i][3] * m.elem[3][j];
                }
            }
            
            return out;
        }

        Vector4 operator*(const Vector4& v)
        {
            Vector4 out;
            
            out.x = elem[0][0] * v.x + elem[0][1] * v.y + elem[0][2] * v.z + elem[0][3] * v.w;
            out.y = elem[1][0] * v.x + elem[1][1] * v.y + elem[1][2] * v.z + elem[1][3] * v.w;
            out.z = elem[2][0] * v.x + elem[2][1] * v.y + elem[2][2] * v.z + elem[2][3] * v.w;
            out.w = elem[3][0] * v.x + elem[3][1] * v.y + elem[3][2] * v.z + elem[3][3] * v.w;

            return out;
        }

        bool operator==(const Matrix4& m)
        {
            for (i32 i = 0; i < 16; ++i)
            {
                if (data[i] != m.data[i])
                    return false;
            }

            return true;
        }

        void identity()
        {
            for (i32 i = 0; i < 16; i++)
                data[i] = 0;

            data[0] = 1.0f;
            data[5] = 1.0f;
            data[10] = 1.0f;
            data[15] = 1.0f;
        }
    };
}

#endif // JPLATFORM_LINUX

#endif // JOJ_MATRIX_H
