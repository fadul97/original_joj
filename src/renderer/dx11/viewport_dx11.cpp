#include "renderer/dx11/viewport_dx11.h"

joj::DX11Viewport::DX11Viewport()
    : Viewport()
{
}

joj::DX11Viewport::DX11Viewport(f32 xtop_left, f32 ytop_left, u16 width, u16 height, f32 min_depth, f32 max_depth)
    : Viewport(xtop_left, ytop_left, width, height, min_depth, max_depth)
{
}

joj::DX11Viewport::~DX11Viewport()
{
}

D3D11_VIEWPORT& joj::DX11Viewport::get_viewport() const
{
	D3D11_VIEWPORT viewport = { 0 };
	viewport.TopLeftY = m_xtop_left;
	viewport.TopLeftX = m_ytop_left;
	viewport.Width = static_cast<f32>(m_width);
	viewport.Height = static_cast<f32>(m_height);
	viewport.MinDepth = m_min_depth;
	viewport.MaxDepth = m_max_depth;
	return viewport;
}