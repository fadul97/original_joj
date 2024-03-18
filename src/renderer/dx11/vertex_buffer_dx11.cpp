#include "renderer/dx11/vertex_buffer_dx11.h"

joj::DX11VertexBuffer::DX11VertexBuffer()
{
    m_vbd = { 0 };
    m_vertex_buffer = nullptr;
    m_init_data = { 0 };
}

joj::DX11VertexBuffer::~DX11VertexBuffer()
{
    // Release vertex buffer
}

void joj::DX11VertexBuffer::build(u32 byte_width, const void* data)
{
    // Buffer to store vertices, describe how data will be accessed and where it will be bound to the rendering pipeline
    m_vbd.Usage = D3D11_USAGE_IMMUTABLE;
    m_vbd.ByteWidth = byte_width;
    m_vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    m_vbd.CPUAccessFlags = 0;
    m_vbd.MiscFlags = 0;
    m_vbd.StructureByteStride = 0;

    // Specify data we want to initialize the buffer contents with
    m_init_data.pSysMem = data;
}