#ifndef JOJ_DX11_DEPTH_STENCIL_MANAGER_H
#define JOJ_DX11_DEPTH_STENCIL_MANAGER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include <d3d11.h>
#include <wrl/client.h>

namespace joj
{
    class JAPI DX11DepthStencilManager
    {
    public:
        DX11DepthStencilManager();
        ~DX11DepthStencilManager();

        void describe_default(u32 width, u32 height, u32 antialising, u32 quality);

        Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& get_depthstencil_view();
        D3D11_DEPTH_STENCIL_VIEW_DESC& get_dsv_desc();

    private:
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthstencil_view;
        ID3D11Texture2D* m_depthstencil_buffer;
        D3D11_DEPTH_STENCIL_VIEW_DESC m_depthstencil_view_desc;
    };

    inline Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& DX11DepthStencilManager::get_depthstencil_view()
    { return m_depthstencil_view; }

    inline D3D11_DEPTH_STENCIL_VIEW_DESC& DX11DepthStencilManager::get_dsv_desc()
    { return m_depthstencil_view_desc; }

} // namespace joj

#if JPLATFORM_WINDOWS

#endif // JOJ_DX11_DEPTH_STENCIL_MANAGER_H