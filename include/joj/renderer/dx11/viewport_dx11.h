#ifndef JOJ_DX11_VIEWPORT_H
#define JOJ_DX11_VIEWPORT_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include "renderer/viewport.h"
#include <wrl/client.h>
#include <dxgi1_6.h>
#include <d3d11.h>

namespace joj
{
    class JAPI DX11Viewport : public Viewport
    {
    public:
        DX11Viewport();
        DX11Viewport(f32 xtop_left, f32 ytop_left, u16 width, u16 height, f32 min_depth, f32 max_depth);
        ~DX11Viewport();

        D3D11_VIEWPORT& get_viewport() const;
    };
}

#endif // JPLATFORM_WINDOWS

#endif // JOJ_DX11_VIEWPORT_H
