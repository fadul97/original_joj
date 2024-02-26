#include "renderer\dx11\renderer_dx11.h"

#include <dxgi.h>
#include <d3dcompiler.h>

joj::DX11Renderer::DX11Renderer()
{
    m_context = std::make_unique<DX11Context>();

    m_device = nullptr;
    m_device_context = nullptr;
    
    m_antialiasing = 1;             // No antialising
	m_quality = 0;                  // Default quality
	m_vsync = false;                // No vertical sync
	m_swap_chain = nullptr;         // Swap chain
	m_render_target_view = nullptr; // Backbuffer render target view
	m_depth_stencil_view = nullptr; // Depth/Stencil view
	m_viewport = { 0 };             // Viewport
	m_blend_state = nullptr;        // Color mix settings
	m_rasterizer_state = nullptr;   // Rasterizer state

    // Background color
	bg_color[0] = 0.0f;		// Red
	bg_color[1] = 0.0f;		// Green
	bg_color[2] = 0.0f;		// Blue
	bg_color[3] = 0.0f;		// Alpha (0 = transparent, 1 = solid)
}

joj::DX11Renderer::~DX11Renderer()
{
    // Release rasterizer state
	if (m_rasterizer_state)
		m_rasterizer_state->Release();

	// Release blend state
	if (m_blend_state)
		m_blend_state->Release();

	// Release depth stencil view
	if (m_depth_stencil_view)
		m_depth_stencil_view->Release();

	// Release render target view
	if (m_render_target_view)
		m_render_target_view->Release();

	// Release swap chain
	if (m_swap_chain)
	{
		// Direct3D is unable to close when full screen
		m_swap_chain->SetFullscreenState(false, NULL);
		m_swap_chain->Release();
	}

	/*
	//  Release graphics device context
	if (m_device_context)
	{
		// Restore to original state
		m_device_context->ClearState();
		m_device_context->Flush();
		m_device_context->Release();
		m_device_context = nullptr;
	}

	// Release graphics device
	if (m_device)
	{
		m_device->Release();
		m_device = nullptr;
	}
	*/

	m_context->get_debug()->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	printf("\n");
}


b8 joj::DX11Renderer::init(std::unique_ptr<Win32Window>& window)
{
    // Background color of the backbuffer = window background color
	COLORREF color = window->get_color();

	bg_color[0] = GetRValue(color) / 255.0f;
	bg_color[1] = GetGValue(color) / 255.0f;
	bg_color[2] = GetBValue(color) / 255.0f;
	bg_color[3] = 1.0f;

    // Initialize DX11 context
	if (!m_context->create(window))
	{
		// TODO: Use own logger and return value
		printf("Failed to initialize DX11 context.\n");
		return false;
	}

    // FIXME: Check make_current method
    m_context->make_current(window);

    // Get pointers to D3D11 Device and D3D11 Device Context
	m_device = m_context->get_device();
	m_device_context = m_context->get_device_context();

    // ------------------------------------------------------------------------------------------------------
    //                                          PIPELINE SETUP
    // ------------------------------------------------------------------------------------------------------

    // Describe Swap Chain
	DXGI_SWAP_CHAIN_DESC swap_chain_desc = { 0 };
	swap_chain_desc.BufferDesc.Width = u32(window->get_width());							// Back buffer width
	swap_chain_desc.BufferDesc.Height = u32(window->get_height());							// Back buffer height
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;									// Refresh rate in hertz 
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 1;									// Numerator is an int
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;							// Color format - RGBA 8 bits
	swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;		// Default value for Flags
	swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;						// Default mode for scaling
	swap_chain_desc.SampleDesc.Count = m_antialiasing;										// Samples per pixel (antialiasing)
	swap_chain_desc.SampleDesc.Quality = m_quality;											// Level of image quality
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;							// Use surface as Render Target
	swap_chain_desc.BufferCount = 2;														// Number of buffers (Front + Back)
	swap_chain_desc.OutputWindow = window->get_id();										// Window ID
	swap_chain_desc.Windowed = (window->get_mode() == joj::WindowMode::WINDOWED);           // Fullscreen or windowed 
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;								// Discard surface after presenting
	swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;							// Use Back buffer size for Fullscreen

	// Create Swap Chain
	if (m_context->get_factory()->CreateSwapChain(m_device.Get(), &swap_chain_desc, &m_swap_chain) != S_OK)
	{
		// TODO: Use own logger and return value
		printf("Failed to CreateSwapChain.\n");
		return false;
	}

    // ---------------------------------------------------
	// Render Target View
	// ---------------------------------------------------

    // Get backbuffer surface of a Swap Chain
	ID3D11Texture2D* backbuffer = nullptr;
	if (m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backbuffer)) != S_OK)
	{
		// TODO: Use own logger and return value
		printf("Failed to Get backbuffer surface of a Swap Chain.\n");
		return false;
	}

	// Create render target view for backbuffer
	if (m_device->CreateRenderTargetView(backbuffer, NULL, &m_render_target_view) != S_OK)
	{
		// TODO: Use own logger and return value
		printf("Failed to CreateRenderTargetView.\n");
		return false;
	}

    // ---------------------------------------------------
	// Depth/Stencil View
	// ---------------------------------------------------

	// Describe Depth/Stencil Buffer Desc
	D3D11_TEXTURE2D_DESC depth_stencil_desc = { 0 };
	depth_stencil_desc.Width = u32(window->get_width());		// Depth/Stencil buffer width
	depth_stencil_desc.Height = u32(window->get_height());		// Depth/Stencil buffer height
	depth_stencil_desc.MipLevels = 0;							// Number of mipmap levels
	depth_stencil_desc.ArraySize = 1;							// Number of textures in array
	depth_stencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	// Color format - Does it need to be the same format of swapChainDesc?
	depth_stencil_desc.SampleDesc.Count = m_antialiasing;			// Samples per pixel (antialiasing)
	depth_stencil_desc.SampleDesc.Quality = m_quality;			// Level of image quality
	depth_stencil_desc.Usage = D3D11_USAGE_DEFAULT;				// Default - GPU will both read and write to the resource
	depth_stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;	// Where resource will be bound to the pipeline
	depth_stencil_desc.CPUAccessFlags = 0;						// CPU will not read not write to the Depth/Stencil buffer
	depth_stencil_desc.MiscFlags = 0;							// Optional flags

	// Create Depth/Stencil Buffer
	ID3D11Texture2D* depth_stencil_buffer;
	if (m_device->CreateTexture2D(&depth_stencil_desc, 0, &depth_stencil_buffer) != S_OK)
	{
		// TODO: Use own logger and return value
		printf("Failed to CreateTexture2D.\n");
		return false;
	}

	// Create Depth/Stencil View
	if (m_device->CreateDepthStencilView(depth_stencil_buffer, 0, &m_depth_stencil_view) != S_OK)
	{
		// TODO: Use own logger and return value
		printf("Failed to CreateDepthStencilView.\n");
		return false;
	}

	// Bind render target and depth stencil to the Output Merger stage
	m_device_context->OMSetRenderTargets(1, &m_render_target_view, m_depth_stencil_view);

    // ---------------------------------------------------
	// Viewport
	// ---------------------------------------------------

	// Describe Viewport
	m_viewport.TopLeftY = 0.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.Width = static_cast<f32>(window->get_width());
	m_viewport.Height = static_cast<f32>(window->get_height());
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;

	// Set Viewport
	m_device_context->RSSetViewports(1, &m_viewport);

    // ---------------------------------------------
	// Blend State
	// ---------------------------------------------

	// Describe blend state
	D3D11_BLEND_DESC blend_desc = { };
	blend_desc.AlphaToCoverageEnable = false;                                // Highlight the silhouette of sprites
	blend_desc.IndependentBlendEnable = false;                               // Use the same mix for all render targets
	blend_desc.RenderTarget[0].BlendEnable = true;                           // Enable blending
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;             // Source mixing factor
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;        // Target of RGB mixing is inverted alpha
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;                 // Addition operation in color mixing
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;        // Alpha blend source is the alpha of the pixel shader
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;   // Fate of Alpha mixture is inverted alpha
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;            // Addition operation in color mixing
	blend_desc.RenderTarget[0].RenderTargetWriteMask = 0x0F;                 // Components of each pixel that can be overwritten

	// Create blend state
	if (m_device->CreateBlendState(&blend_desc, &m_blend_state) != S_OK)
	{
		// TODO: Use own logger and return value
		printf("Failed to CreateBlendState.\n");
		return false;
	}

	// Bind blend state to the Output Merger stage
	m_device_context->OMSetBlendState(m_blend_state, nullptr, 0xffffffff);

	// ---------------------------------------------------
	// Rasterizer
	// ---------------------------------------------------

	// TODO: comment specifications on rasterizer
	// Describe rasterizer
	D3D11_RASTERIZER_DESC rasterizer_desc = {};
	ZeroMemory(&rasterizer_desc, sizeof(rasterizer_desc));
	//rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizer_desc.CullMode = D3D11_CULL_BACK;
	//rasterizer_desc.CullMode = D3D11_CULL_NONE;
	rasterizer_desc.DepthClipEnable = true;

	// Create rasterizer state
	if (m_device->CreateRasterizerState(&rasterizer_desc, &m_rasterizer_state) != S_OK)
	{
		// TODO: Use own logger and return value
		printf("Failed to CreateRasterizerState.\n");
		return false;
	}

	// Set rasterizer state
	m_device_context->RSSetState(m_rasterizer_state);

	// ---------------------------------------------------
	//	Release Resources
	// ---------------------------------------------------

	backbuffer->Release();
	depth_stencil_buffer->Release();

    printf("DX11Renderer initialized.\n");

	// TODO: Use own return value
	return true;
}

void joj::DX11Renderer::render()
{
    
}

void joj::DX11Renderer::clear(f32 r, f32 g, f32 b, f32 a)
{
    bg_color[0] = r;
    bg_color[1] = g;
    bg_color[2] = b;
    bg_color[3] = a;
    m_device_context->ClearRenderTargetView(m_render_target_view, bg_color);
	m_device_context->ClearDepthStencilView(m_depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void joj::DX11Renderer::swap_buffers()
{
    m_swap_chain->Present(m_vsync, NULL);
	m_device_context->OMSetRenderTargets(1, &m_render_target_view, m_depth_stencil_view);
}

void joj::DX11Renderer::shutdown()
{
    
}
