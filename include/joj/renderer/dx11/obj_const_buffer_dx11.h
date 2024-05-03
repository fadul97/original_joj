#ifndef JOJ_DX11_OBJECT_CONSTANT_BUFFER_H
#define JOJ_DX11_OBJECT_CONSTANT_BUFFER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include <WRL/client.h>
#include "math/matrix4.h"

namespace joj
{
    struct WVPObjConstant
    {
        DirectX::XMFLOAT4X4 wvp =
        {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    };

    class JAPI DX11ObjectConstantBuffer
    {
    public:
        DX11ObjectConstantBuffer();
        ~DX11ObjectConstantBuffer();

        void build(u32 byte_wdith, Matrix4 matrix);
        void set_matrix(Matrix4 matrix, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& device_context);

        const D3D11_BUFFER_DESC* get_const_buffer_desc() const;
        const D3D11_SUBRESOURCE_DATA* get_subdata() const;
        Microsoft::WRL::ComPtr<ID3D11Buffer>& get_const_buffer();

    private:
        D3D11_BUFFER_DESC m_const_buffer_desc;
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_constant_buffer;
        D3D11_SUBRESOURCE_DATA m_constant_data;
    };

    inline const D3D11_BUFFER_DESC* DX11ObjectConstantBuffer::get_const_buffer_desc() const
    { return &m_const_buffer_desc; }

    inline const D3D11_SUBRESOURCE_DATA* DX11ObjectConstantBuffer::get_subdata() const
    { return &m_constant_data; }

    inline Microsoft::WRL::ComPtr<ID3D11Buffer>& DX11ObjectConstantBuffer::get_const_buffer()
    { return m_constant_buffer; }
}

#endif // JOJ_DX11_OBJECT_CONSTANT_BUFFER_H