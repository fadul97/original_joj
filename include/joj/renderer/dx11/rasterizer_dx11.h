#ifndef JOJ_DX11_RASTERIZER_H
#define JOJ_DX11_RASTERIZER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include <wrl/client.h>
#include <d3d11.h>

namespace joj
{
    class JAPI DX11Rasterizer
    {
    public:
        DX11Rasterizer();
        ~DX11Rasterizer();

        Microsoft::WRL::ComPtr<ID3D11RasterizerState>& get_rasterizer_state();
        D3D11_RASTERIZER_DESC& get_rasterizer_desc();
        
        void describe_default();
        void set_fill_mode(D3D11_FILL_MODE mode);
        void set_cull_mode(D3D11_CULL_MODE mode);
        
    private:
        Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizer_state;
        D3D11_RASTERIZER_DESC m_rasterizer_desc;
    };

    inline Microsoft::WRL::ComPtr<ID3D11RasterizerState>& DX11Rasterizer::get_rasterizer_state()
    { return m_rasterizer_state; }

    inline D3D11_RASTERIZER_DESC& DX11Rasterizer::get_rasterizer_desc()
    { return m_rasterizer_desc; }

    inline void DX11Rasterizer::set_fill_mode(D3D11_FILL_MODE mode)
    { m_rasterizer_desc.FillMode = mode; }

    inline void DX11Rasterizer::set_cull_mode(D3D11_CULL_MODE mode)
    { m_rasterizer_desc.CullMode = mode; }
}

#endif // JPLATFORM_WINDOWS

#endif // JOJ_DX11_RASTERIZER_H
