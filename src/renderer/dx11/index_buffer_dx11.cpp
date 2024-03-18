#include "renderer/dx11/index_buffer_dx11.h"

joj::DX11IndexBuffer::DX11IndexBuffer()
{
    m_ibd = { 0 };
    m_index_buffer = nullptr;
    m_init_data = { 0 };
}

joj::DX11IndexBuffer::~DX11IndexBuffer()
{
    // Release vertex buffer
}

void joj::DX11IndexBuffer::build(u32 byte_width, const void* data)
{
    // Buffer to store vertices, describe how data will be accessed and where it will be bound to the rendering pipeline
    m_ibd.Usage = D3D11_USAGE_IMMUTABLE;
    m_ibd.ByteWidth = byte_width;
    m_ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    m_ibd.CPUAccessFlags = 0;
    m_ibd.MiscFlags = 0;
    m_ibd.StructureByteStride = 0;

    // Specify data we want to initialize the buffer contents with
    m_init_data.pSysMem = data;
}