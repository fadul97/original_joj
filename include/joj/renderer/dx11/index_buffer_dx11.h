#ifndef JOJ_DX11_INDEX_BUFFER_H
#define JOJ_DX11_INDEX_BUFFER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <d3d11.h>
#include <wrl/client.h>

namespace joj
{
    class JAPI DX11IndexBuffer
    {
    public:
        DX11IndexBuffer();
        ~DX11IndexBuffer();

        void build(u32 byte_width, const void* data);

        const D3D11_BUFFER_DESC* get_buffer_desc() const;
        Microsoft::WRL::ComPtr<ID3D11Buffer>& get_buffer();
        const Microsoft::WRL::ComPtr<ID3D11Buffer>& get_buffer_const() const;
        const D3D11_SUBRESOURCE_DATA* get_subdata() const;

    private:
        D3D11_BUFFER_DESC m_ibd;
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_index_buffer;
        D3D11_SUBRESOURCE_DATA m_init_data;
    };

    inline const D3D11_BUFFER_DESC* DX11IndexBuffer::get_buffer_desc() const
    { return &m_ibd; }

    inline Microsoft::WRL::ComPtr<ID3D11Buffer>& DX11IndexBuffer::get_buffer()
    { return m_index_buffer; }

    inline const Microsoft::WRL::ComPtr<ID3D11Buffer>& DX11IndexBuffer::get_buffer_const() const
    { return m_index_buffer; }

    inline const D3D11_SUBRESOURCE_DATA* DX11IndexBuffer::get_subdata() const
    { return &m_init_data; }
}

#endif // JOJ_DX11_INDEX_BUFFER_H