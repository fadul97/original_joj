#ifndef JOJ_DX11_RENDERER_H
#define JOJ_DX11_RENDERER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "renderer/renderer.h"
#include "platform/win32/window_win32.h"
#include <memory>
#include "graphics/dx11/context_dx11.h"

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

        void set_primitive_topology(D3D11_PRIMITIVE_TOPOLOGY topology) const;

    private:
        std::unique_ptr<DX11Context> m_context;

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
        IDXGISwapChain* m_swap_chain;                  // Swap chain
        ID3D11RenderTargetView* m_render_target_view;  // Backbuffer render target view
        ID3D11DepthStencilView* m_depth_stencil_view;  // Depth/Stencil view
        D3D11_VIEWPORT m_viewport;                     // Viewport
        ID3D11BlendState* m_blend_state;               // Color mix settings
        ID3D11RasterizerState* m_rasterizer_state;     // Rasterizer state
    };

    inline void DX11Renderer::set_primitive_topology(D3D11_PRIMITIVE_TOPOLOGY topology) const
    { m_device_context->IASetPrimitiveTopology(topology); }
}



#endif // JOJ_DX11_RENDERER_H
