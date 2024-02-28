#include "renderer/dx11/renderer_dx11.h"

#include <dxgi.h>
#include <d3dcompiler.h>
#include "renderer/dx11/viewport_dx11.h"

joj::DX11Renderer::DX11Renderer()
{
    m_context = std::make_unique<DX11Context>();
	m_swapchain = std::make_unique<DX11SwapChain>();

    m_device = nullptr;
    m_device_context = nullptr;
    
	m_antialiasing = 1;             // No antialising
	m_quality = 0;                  // Default quality
	m_vsync = false;                // No vertical sync
	m_render_target_view = nullptr; // Backbuffer render target view
	m_depth_stencil_view = nullptr; // Depth/Stencil view
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
	m_swapchain->get_swapchain()->Present(m_vsync, NULL);
	m_device_context->OMSetRenderTargets(1, &m_render_target_view, m_depth_stencil_view);
}

void joj::DX11Renderer::shutdown()
{
    
}

joj::ErrorCode joj::DX11Renderer::setup_default_pipeline(std::unique_ptr<Win32Window>& window)
{
	// ------------------------------------------------------------------------------------------------------
	//                                          PIPELINE SETUP
	// ------------------------------------------------------------------------------------------------------

	// ---------------------------------------------------
	// Swap Chain
	// ---------------------------------------------------

	// Describe swap chain
	m_swapchain->describe_default(window);

	// Create swap chain
	if (create_swapchain(m_swapchain->get_swapchain_desc(), m_swapchain->get_swapchain().GetAddressOf()) != ErrorCode::OK)
	{
		// TODO: Use own logger and return value
		printf("Failed to CreateSwapChain.\n");
		return ErrorCode::ERR_RENDERER_SWAPCHAIN_CREATION;
	}

	// ---------------------------------------------------
	// Render Target View
	// ---------------------------------------------------

	// Get backbuffer surface of a Swap Chain
	ID3D11Texture2D* backbuffer = nullptr;
	if (get_swapchain_buffer(m_swapchain->get_swapchain(), reinterpret_cast<void**>(&backbuffer)) != ErrorCode::OK)
	{
		// TODO: Use own logger and return value
		printf("Failed to Get backbuffer surface of a Swap Chain.\n");
		return ErrorCode::ERR_RENDERER_SWAPCHAIN_GET_BUFFER;
	}

	// Create render target view for backbuffer
	if (create_rtv(backbuffer, &m_render_target_view) != ErrorCode::OK)
	{
		// TODO: Use own logger and return value
		printf("Failed to CreateRenderTargetView.\n");
		return ErrorCode::ERR_RENDER_TARGET_VIEW_CREATION;
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
	ID3D11Texture2D* depth_stencil_buffer = nullptr;
	if (create_texture2D(depth_stencil_desc, &depth_stencil_buffer) != ErrorCode::OK)
	{
		// TODO: Use own logger and return value
		printf("Failed to CreateTexture2D.\n");
		return ErrorCode::ERR_RENDERER_TEXTURE2D_CREATION;
	}

	// Create Depth/Stencil View
	if (create_dsv(depth_stencil_buffer, &m_depth_stencil_view) != ErrorCode::OK)
	{
		// TODO: Use own logger and return value
		printf("Failed to CreateDepthStencilView.\n");
		return ErrorCode::ERR_RENDERER_DEPTHSTENCIL_VIEW_CREATION;
	}

	// Bind render target and depth stencil to the Output Merger stage
	set_render_targets(&m_render_target_view, m_depth_stencil_view);

	// ---------------------------------------------------
	// Viewport
	// ---------------------------------------------------

	// Describe Viewport
	DX11Viewport viewport{ 0.0f, 0.0f, static_cast<u16>(window->get_width()), static_cast<u16>(window->get_height()), 0.0f, 1.0f };
	D3D11_VIEWPORT vp = viewport.get_viewport();

	// Set Viewport
	set_viewport(&vp);

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
	if (create_blend_state(&blend_desc, &m_blend_state) != ErrorCode::OK)
	{
		// TODO: Use own logger and return value
		printf("Failed to CreateBlendState.\n");
		return ErrorCode::ERR_RENDERER_BLEND_STATE_CREATION;
	}

	// Bind blend state to the Output Merger stage
	set_blend_state(m_blend_state);

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

	// Create rasterizer state(
	if (create_rasterizer_state(&rasterizer_desc, &m_rasterizer_state) != ErrorCode::OK)
	{
		// TODO: Use own logger and return value
		printf("Failed to CreateRasterizerState.\n");
		return ErrorCode::ERR_RENDERER_RASTERIZER_CREATION;
	}

	// Set rasterizer state
	set_rasterizer_state(m_rasterizer_state);

	// ---------------------------------------------------
	//	Release Resources
	// ---------------------------------------------------

	backbuffer->Release();
	depth_stencil_buffer->Release();

	// TODO: Use own return value
	return ErrorCode::OK;
}

joj::ErrorCode joj::DX11Renderer::create_swapchain(DXGI_SWAP_CHAIN_DESC& swapchain_desc, IDXGISwapChain** swapchain)
{
	/*
	HRESULT hr = m_context->get_factory()->CreateSwapChain(m_device.Get(), &swapchain_desc, swapchain);
	_com_error err(hr);
	LPCTSTR errMsg = err.ErrorMessage();
	printf("errMsg = %s\n", errMsg);
	*/

	if (FAILED(m_context->get_factory()->CreateSwapChain(m_device.Get(), &swapchain_desc, swapchain)))
	{
		// TODO: Use own logger
		const char* err_str = error_to_string(ErrorCode::ERR_RENDERER_SWAPCHAIN_CREATION);
		printf("[%s]: Failed to CreateSwapChain.\n", err_str);
		return ErrorCode::ERR_RENDERER_SWAPCHAIN_CREATION;
	}

	return ErrorCode::OK;
}

// FIXME: buffer remains NULL after method
joj::ErrorCode joj::DX11Renderer::get_swapchain_buffer(Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain, void** buffer)
{
	if (FAILED(swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), buffer)))
	{
		// TODO: Use own logger
		const char* err_str = error_to_string(ErrorCode::ERR_RENDERER_SWAPCHAIN_GET_BUFFER);
		printf("[%s]: Failed to get buffer surface of Swap Chain.\n", err_str);
		return ErrorCode::ERR_RENDERER_SWAPCHAIN_GET_BUFFER;
	}

	return ErrorCode::OK;
}

joj::ErrorCode joj::DX11Renderer::create_rtv(ID3D11Texture2D* buffer, ID3D11RenderTargetView** rtv)
{
	if (FAILED(m_device->CreateRenderTargetView(buffer, NULL, rtv)))
	{
		// TODO: Use own logger
		const char* err_str = error_to_string(ErrorCode::ERR_RENDER_TARGET_VIEW_CREATION);
		printf("[%s]: Failed to CreateRenderTargetView.\n", err_str);
		return ErrorCode::ERR_RENDER_TARGET_VIEW_CREATION;
	}

	return ErrorCode::OK;
}

joj::ErrorCode joj::DX11Renderer::create_texture2D(const D3D11_TEXTURE2D_DESC& depthstencil_desc, ID3D11Texture2D** depthstencil_buffer)
{
	if (FAILED(m_device->CreateTexture2D(&depthstencil_desc, 0, depthstencil_buffer)))
	{
		// TODO: Use own logger
		const char* err_str = error_to_string(ErrorCode::ERR_RENDERER_TEXTURE2D_CREATION);
		printf("[%s]: Failed to CreateTexture2D.\n", err_str);
		return ErrorCode::ERR_RENDERER_TEXTURE2D_CREATION;
	}

	return ErrorCode::OK;
}

joj::ErrorCode joj::DX11Renderer::create_dsv(ID3D11Resource* depthstencil_buffer, ID3D11DepthStencilView** dsv)
{
	if (FAILED(m_device->CreateDepthStencilView(depthstencil_buffer, 0, &m_depth_stencil_view)))
	{
		// TODO: Use own logger
		const char* err_str = error_to_string(ErrorCode::ERR_RENDERER_DEPTHSTENCIL_VIEW_CREATION);
		printf("[%s]: Failed to CreateDepthStencilView.\n", err_str);
		return ErrorCode::ERR_RENDERER_DEPTHSTENCIL_VIEW_CREATION;
	}

	return ErrorCode::OK;
}

void joj::DX11Renderer::set_render_targets(ID3D11RenderTargetView* const* rtv, ID3D11DepthStencilView* dsv)
{
	m_device_context->OMSetRenderTargets(1, rtv, dsv);
}

void joj::DX11Renderer::set_viewport(const D3D11_VIEWPORT* viewport)
{
	m_device_context->RSSetViewports(1, viewport);
}

joj::ErrorCode joj::DX11Renderer::create_blend_state(const D3D11_BLEND_DESC* blend_state_desc, ID3D11BlendState** blend_state)
{
	if (FAILED(m_device->CreateBlendState(blend_state_desc, blend_state)))
	{
		// TODO: Use own logger
		const char* err_str = error_to_string(ErrorCode::ERR_RENDERER_BLEND_STATE_CREATION);
		printf("[%s]: Failed to CreateBlendState.\n", err_str);
		return ErrorCode::ERR_RENDERER_BLEND_STATE_CREATION;
	}

	return ErrorCode::OK;
}

void joj::DX11Renderer::set_blend_state(ID3D11BlendState* blend_state)
{
	m_device_context->OMSetBlendState(blend_state, nullptr, 0xffffffff);
}

joj::ErrorCode joj::DX11Renderer::create_rasterizer_state(const D3D11_RASTERIZER_DESC* rasterizer_desc, ID3D11RasterizerState** rasterizer_state)
{
	if (FAILED(m_device->CreateRasterizerState(rasterizer_desc, rasterizer_state)))
	{
		// TODO: Use own logger
		const char* err_str = error_to_string(ErrorCode::ERR_RENDERER_RASTERIZER_CREATION);
		printf("[%s]: Failed to CreateRasterizerState.\n", err_str);
		return ErrorCode::ERR_RENDERER_RASTERIZER_CREATION;
	}

	return ErrorCode::OK;
}

void joj::DX11Renderer::set_rasterizer_state(ID3D11RasterizerState* rasterizer_state)
{
	m_device_context->RSSetState(rasterizer_state);
}