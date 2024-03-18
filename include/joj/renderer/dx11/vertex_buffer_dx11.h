#ifndef JOJ_DX11_VERTEX_BUFFER_H
#define JOJ_DX11_VERTEX_BUFFER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <d3d11.h>
#include <wrl/client.h>

namespace joj
{
    class JAPI DX11VertexBuffer
    {
    public:
        DX11VertexBuffer();
        ~DX11VertexBuffer();

        void build(u32 byte_width, const void* data);

        const D3D11_BUFFER_DESC* get_buffer_desc() const;
        Microsoft::WRL::ComPtr<ID3D11Buffer>& get_buffer();
        const Microsoft::WRL::ComPtr<ID3D11Buffer>& get_buffer_const() const;
        const D3D11_SUBRESOURCE_DATA* get_subdata() const;
    
    private:
        D3D11_BUFFER_DESC m_vbd;
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertex_buffer;
        D3D11_SUBRESOURCE_DATA m_init_data;
    };

    inline const D3D11_BUFFER_DESC* DX11VertexBuffer::get_buffer_desc() const
    { return &m_vbd; }

    inline Microsoft::WRL::ComPtr<ID3D11Buffer>& DX11VertexBuffer::get_buffer()
    { return m_vertex_buffer; }

    inline const Microsoft::WRL::ComPtr<ID3D11Buffer>& DX11VertexBuffer::get_buffer_const() const
    { return m_vertex_buffer; }

    inline const D3D11_SUBRESOURCE_DATA* DX11VertexBuffer::get_subdata() const
    { return &m_init_data; }
}

#endif // JOJ_DX11_VERTEX_BUFFER_H