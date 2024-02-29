#ifndef JOJ_DX11_BLEND_STATE_H
#define JOJ_DX11_BLEND_STATE_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include <d3d11.h>
#include <wrl/client.h>

namespace joj
{
    class JAPI DX11BlendState
    {
    public:
        DX11BlendState();
        ~DX11BlendState();

        void describe_default();

        Microsoft::WRL::ComPtr<ID3D11BlendState>& get_blend_state();
        const D3D11_BLEND_DESC& get_blend_desc() const;

    private:
        Microsoft::WRL::ComPtr<ID3D11BlendState> m_blend_state;
        D3D11_BLEND_DESC m_blend_desc;
    };

    inline Microsoft::WRL::ComPtr<ID3D11BlendState>& DX11BlendState::get_blend_state()
    { return m_blend_state; }

    inline const D3D11_BLEND_DESC& DX11BlendState::get_blend_desc() const
    { return m_blend_desc; }

} // namespace joj

#endif // JPLATFORM_WINDOWS

#endif // JOJ_DX11_BLEND_STATE_H