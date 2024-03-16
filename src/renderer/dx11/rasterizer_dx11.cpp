#include "renderer/dx11/rasterizer_dx11.h"

joj::DX11Rasterizer::DX11Rasterizer()
{
	m_rasterizer_state = nullptr;
	m_rasterizer_desc = { };
}

joj::DX11Rasterizer::~DX11Rasterizer()
{

}

void joj::DX11Rasterizer::describe_default()
{
	ZeroMemory(&m_rasterizer_desc, sizeof(m_rasterizer_desc));
	
	//m_rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	//m_rasterizer_desc.CullMode = D3D11_CULL_NONE;
	
	m_rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
	m_rasterizer_desc.CullMode = D3D11_CULL_BACK;

	m_rasterizer_desc.DepthClipEnable = true;
}