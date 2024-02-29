#include "renderer/dx11/blend_state_dx11.h"

joj::DX11BlendState::DX11BlendState()
{
	m_blend_state = nullptr;
	m_blend_desc = { 0 };
}

joj::DX11BlendState::~DX11BlendState()
{

}

void joj::DX11BlendState::describe_default()
{
	m_blend_desc.AlphaToCoverageEnable = false;                                // Highlight the silhouette of sprites
	m_blend_desc.IndependentBlendEnable = false;                               // Use the same mix for all render targets
	m_blend_desc.RenderTarget[0].BlendEnable = true;                           // Enable blending
	m_blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;             // Source mixing factor
	m_blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;        // Target of RGB mixing is inverted alpha
	m_blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;                 // Addition operation in color mixing
	m_blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;        // Alpha blend source is the alpha of the pixel shader
	m_blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;   // Fate of Alpha mixture is inverted alpha
	m_blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;            // Addition operation in color mixing
	m_blend_desc.RenderTarget[0].RenderTargetWriteMask = 0x0F;                 // Components of each pixel that can be overwritten
}