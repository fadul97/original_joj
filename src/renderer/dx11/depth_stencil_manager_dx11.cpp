#include "renderer/dx11/depth_stencil_manager_dx11.h"

joj::DX11DepthStencilManager::DX11DepthStencilManager()
{
    m_depthstencil_view = nullptr;
    m_depthstencil_view_desc = { 0 };
}

joj::DX11DepthStencilManager::~DX11DepthStencilManager()
{

}

void joj::DX11DepthStencilManager::describe_default(u32 width, u32 height, u32 antialising, u32 quality)
{
	m_depthstencil_view_desc.Width = width;                           // Depth/Stencil buffer width
	m_depthstencil_view_desc.Height = height;                         // Depth/Stencil buffer height
	m_depthstencil_view_desc.MipLevels = 0;                           // Number of mipmap levels
	m_depthstencil_view_desc.ArraySize = 1;                           // Number of textures in array
	m_depthstencil_view_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;  // Color format - Does it need to be the same format of swapChainDesc?
	m_depthstencil_view_desc.SampleDesc.Count = antialising;		    // Samples per pixel (antialiasing)
	m_depthstencil_view_desc.SampleDesc.Quality = quality;            // Level of image quality
	m_depthstencil_view_desc.Usage = D3D11_USAGE_DEFAULT;             // Default - GPU will both read and write to the resource
	m_depthstencil_view_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;    // Where resource will be bound to the pipeline
	m_depthstencil_view_desc.CPUAccessFlags = 0;                      // CPU will not read not write to the Depth/Stencil buffer
	m_depthstencil_view_desc.MiscFlags = 0;                           // Optional flags
}