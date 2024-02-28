#include "renderer/dx11/swapchain_dx11.h"

joj::DX11SwapChain::DX11SwapChain()
{
    m_swapchain = nullptr;
	m_swapchain_desc = { 0 };
	m_antialiasing = 1;
	m_quality = 0;
}

joj::DX11SwapChain::~DX11SwapChain()
{
	if (m_swapchain)
		m_swapchain->SetFullscreenState(false, NULL);
}

void joj::DX11SwapChain::describe_default(std::unique_ptr<Win32Window>& window)
{
	// Describe Swap Chain
	m_swapchain_desc.BufferDesc.Width = u32(window->get_width());                          // Back buffer width
	m_swapchain_desc.BufferDesc.Height = u32(window->get_height());                        // Back buffer height
	m_swapchain_desc.BufferDesc.RefreshRate.Numerator = 60;                                // Refresh rate in hertz 
	m_swapchain_desc.BufferDesc.RefreshRate.Numerator = 1;                                 // Numerator is an int
	m_swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;                       // Color format - RGBA 8 bits
	m_swapchain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;   // Default value for Flags
	m_swapchain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;                   // Default mode for scaling
	m_swapchain_desc.SampleDesc.Count = m_antialiasing;                                    // Samples per pixel (antialiasing)
	m_swapchain_desc.SampleDesc.Quality = m_quality;                                       // Level of image quality
	m_swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;                        // Use surface as Render Target
	m_swapchain_desc.BufferCount = 2;                                                      // Number of buffers (Front + Back)
	m_swapchain_desc.OutputWindow = window->get_id();                                      // Window ID
	m_swapchain_desc.Windowed = (window->get_mode() == joj::WindowMode::WINDOWED);         // Fullscreen or windowed 
	m_swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;                           // Discard surface after presenting
	m_swapchain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;                       // Use Back buffer size for Fullscreen
}
