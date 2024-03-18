#include "renderer/dx11/obj_const_buffer_dx11.h"

joj::DX11ObjectConstantBuffer::DX11ObjectConstantBuffer()
{
    m_constant_data = { 0 };
    m_constant_buffer = nullptr;
    m_const_buffer_desc = { 0 };
}

joj::DX11ObjectConstantBuffer::~DX11ObjectConstantBuffer()
{
    // Release matrix constant buffer
    if (m_constant_buffer)
    {
        m_constant_buffer->Release();
        m_constant_buffer = nullptr;
    }
}

void joj::DX11ObjectConstantBuffer::build(u32 byte_wdith, Matrix4 matrix)
{
    m_const_buffer_desc.ByteWidth = byte_wdith;
    m_const_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
    m_const_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    m_const_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    DirectX::XMMATRIX world_view_proj = DirectX::XMMatrixTranspose(matrix);
    m_constant_data.pSysMem = &world_view_proj;
}
void joj::DX11ObjectConstantBuffer::set_matrix(Matrix4 matrix, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& device_context)
{
    // Update constant buffer with combined matrix (Word-View-Projection Matrix)
    WVPObjConstant objc;
    XMStoreFloat4x4(&objc.wvp, DirectX::XMMatrixTranspose(matrix));
    m_constant_data.pSysMem = &objc.wvp;

    // Get a pointer to the constant buffer data and lock it
    D3D11_MAPPED_SUBRESOURCE mapped_buffer = {};
    device_context->Map(m_constant_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_buffer);

    // Copy the new data to the constant buffer data.
    memcpy(mapped_buffer.pData, &objc, sizeof(WVPObjConstant));

    // Release the pointer to the constant buffer data.
    device_context->Unmap(m_constant_buffer.Get(), 0);
}
