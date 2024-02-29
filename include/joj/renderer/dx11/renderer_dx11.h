#ifndef JOJ_DX11_RENDERER_H
#define JOJ_DX11_RENDERER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include "renderer/renderer.h"
#include "platform/win32/window_win32.h"
#include <memory>
#include "graphics/dx11/context_dx11.h"
#include "swapchain_dx11.h"
#include "error.h"
#include "depth_stencil_manager_dx11.h"
#include "blend_state_dx11.h"
#include "rasterizer_dx11.h"

namespace joj
{
    class JAPI DX11Renderer : public Renderer<Win32Window>
    {
    public:
        DX11Renderer();
        ~DX11Renderer();
        
        b8 init(std::unique_ptr<Win32Window>& window) override;
        void render() override;
        void clear(f32 r = 0.0f, f32 g = 0.0f, f32 b = 0.0f, f32 a = 0.0f) override;
        void swap_buffers() override;
        void shutdown() override;

        ErrorCode setup_default_pipeline(std::unique_ptr<Win32Window>& window);

        ErrorCode create_swapchain(DXGI_SWAP_CHAIN_DESC& swapchain_desc, IDXGISwapChain** swapchain);
        ErrorCode get_swapchain_buffer(Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain, void** buffer);
        ErrorCode create_rtv(ID3D11Texture2D* buffer, ID3D11RenderTargetView** rtv);
        ErrorCode create_texture2D(const D3D11_TEXTURE2D_DESC& depthstencil_desc, ID3D11Texture2D** depthstencil_buffer);
        ErrorCode create_dsv(ID3D11Resource* depthstencil_buffer, ID3D11DepthStencilView** dsv);
        void set_render_targets(ID3D11RenderTargetView* const* rtv, ID3D11DepthStencilView* dsv);
        void set_viewport(const D3D11_VIEWPORT* viewport);
        ErrorCode create_blend_state(const D3D11_BLEND_DESC* blend_state_desc, ID3D11BlendState** blend_state);
        void set_blend_state(ID3D11BlendState* blend_state);
        ErrorCode create_rasterizer_state(const D3D11_RASTERIZER_DESC* rasterizer_desc, ID3D11RasterizerState** rasterizer_state);
        void set_rasterizer_state(ID3D11RasterizerState* rasterizer_state);

        void set_primitive_topology(D3D11_PRIMITIVE_TOPOLOGY topology) const;

    private:
        std::unique_ptr<DX11Context> m_context;
        std::unique_ptr<DX11SwapChain> m_swapchain;
        std::unique_ptr<DX11DepthStencilManager> m_ds_manager;
        std::unique_ptr<DX11BlendState> m_blend;
        std::unique_ptr<DX11Rasterizer> m_rasterizer;

        f32 bg_color[4];

        Microsoft::WRL::ComPtr<ID3D11Device> m_device;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_device_context;

        /**
         * @brief Pipeline  members
         * 
         */

        u32 m_antialiasing;                            // Number of samples for each pixel on the screen
        u32 m_quality;                                 // Antialiasing sampling quality
        b8 m_vsync;                                    // Vertical sync 
        ID3D11RenderTargetView* m_render_target_view;  // Backbuffer render target view
    };

    inline void DX11Renderer::set_primitive_topology(D3D11_PRIMITIVE_TOPOLOGY topology) const
    { m_device_context->IASetPrimitiveTopology(topology); }
}

#endif // JPLATFORM_WINDOWS

#endif // JOJ_DX11_RENDERER_H
