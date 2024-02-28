// TODO: Test class

#ifndef JOJ_DX11_SWAP_CHAIN_H
#define JOJ_DX11_SWAP_CHAIN_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#if JPLATFORM_WINDOWS

#include <wrl/client.h>
#include <dxgi1_6.h>
#include <d3d11.h>
#include "platform/win32/window_win32.h"
#include <memory>

namespace joj
{
    class JAPI DX11SwapChain
    {
    public:
        DX11SwapChain();
        ~DX11SwapChain();

        Microsoft::WRL::ComPtr<IDXGISwapChain>& get_swapchain();
        const Microsoft::WRL::ComPtr<IDXGISwapChain>& get_swapchain_const() const;
        DXGI_SWAP_CHAIN_DESC& get_swapchain_desc();
        
        void describe_default(std::unique_ptr<Win32Window>& window);
        
    private:
    	DXGI_SWAP_CHAIN_DESC m_swapchain_desc;
        Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapchain;
        u32 m_antialiasing;
        u32 m_quality;
    };

    inline Microsoft::WRL::ComPtr<IDXGISwapChain>& DX11SwapChain::get_swapchain()
    { return m_swapchain; }

    inline const Microsoft::WRL::ComPtr<IDXGISwapChain>& DX11SwapChain::get_swapchain_const() const
    { return m_swapchain; }

    inline DXGI_SWAP_CHAIN_DESC& DX11SwapChain::get_swapchain_desc()
    { return m_swapchain_desc; }
}

#endif // JPLATFORM_WINDOWS

#endif // JOJ_DX11_SWAP_CHAIN_H
