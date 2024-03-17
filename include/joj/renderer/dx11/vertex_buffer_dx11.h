#ifndef JOJ_DX11_VERTEX_BUFFER_H
#define JOJ_DX11_VERTEX_BUFFER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <d3d11.h>

namespace joj
{
    class JAPI DX11VertexBuffer
    {
    public:
        DX11VertexBuffer();
        ~DX11VertexBuffer();

        void build(u32 byte_width, const void* data);

        const D3D11_BUFFER_DESC* get_buffer_desc() const;
        ID3D11Buffer* get_buffer() const;
        ID3D11Buffer* const* get_const_buffer() const;
        const D3D11_SUBRESOURCE_DATA* get_subdata() const;
    
    private:
        D3D11_BUFFER_DESC m_vbd;
        ID3D11Buffer* m_vertex_buffer;
        D3D11_SUBRESOURCE_DATA m_init_data;
    };

    inline const D3D11_BUFFER_DESC* DX11VertexBuffer::get_buffer_desc() const
    { return &m_vbd; }

    inline ID3D11Buffer* DX11VertexBuffer::get_buffer() const
    { return m_vertex_buffer; }

    inline ID3D11Buffer* const* DX11VertexBuffer::get_const_buffer() const
    { return &m_vertex_buffer; }

    inline const D3D11_SUBRESOURCE_DATA* DX11VertexBuffer::get_subdata() const
    { return &m_init_data; }
}

#endif // JOJ_DX11_VERTEX_BUFFER_H