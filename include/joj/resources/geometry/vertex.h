#ifndef JOJ_VERTEX_H
#define JOJ_VERTEX_H

#if JPLATFORM_WINDOWS
#include <DirectXMath.h>
namespace joj
{
#define J_PI DirectX::XM_PI
    using Vector3 = DirectX::XMFLOAT3;
    using Vector4 = DirectX::XMFLOAT4;
}
#else
#include "math/jmath.h"
#endif

namespace joj
{
    struct Vertex
    {
        Vector3 pos;
        Vector4 color;
    };
}


#endif // JOJ_VERTEX_H
