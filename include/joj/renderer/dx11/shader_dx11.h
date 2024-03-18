#ifndef JOJ_DX11_SHADER_H
#define JOJ_DX11_SHADER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "renderer/shader.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <WRL/client.h>
#include <math/matrix4.h>
#include "error.h"

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

    class JAPI DX11Shader : public Shader
    {
    public:
        DX11Shader();
        DX11Shader(LPCWSTR vertex_path, LPCWSTR pixel_path, Microsoft::WRL::ComPtr<ID3D11Device>& device);
        ~DX11Shader();

        b8 compile_shaders(LPCWSTR vertex_path, LPCWSTR pixel_path, Microsoft::WRL::ComPtr<ID3D11Device>& device);
        void use() const override;
        void destroy() override;

        void setup_const_buffer(u32 byte_wdith, Matrix4 matrix);

        void set_matrix(Matrix4 matrix, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& device_context);

        ID3D11VertexShader* get_vertex_shader();
        ID3D11PixelShader* get_pixel_shader();
        ID3D11InputLayout* get_input_layout();

        const D3D11_BUFFER_DESC* get_const_buffer_desc() const;
        const D3D11_SUBRESOURCE_DATA* get_subdata() const;
        Microsoft::WRL::ComPtr<ID3D11Buffer>& get_const_buffer();

    private:
        ID3D11VertexShader* m_vertex_shader;
        ID3D11PixelShader* m_pixel_shader;
        ID3D11InputLayout* m_layout;

        // TODO: Should shader class have constant buffer attributes?
        D3D11_BUFFER_DESC m_const_buffer_desc;
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_constant_buffer;
        D3D11_SUBRESOURCE_DATA m_constant_data;
    };

    inline ID3D11VertexShader* DX11Shader::get_vertex_shader()
    { return m_vertex_shader; }

    inline ID3D11PixelShader* DX11Shader::get_pixel_shader()
    { return m_pixel_shader; }

    inline ID3D11InputLayout* DX11Shader::get_input_layout()
    { return m_layout; }

    inline const D3D11_BUFFER_DESC* DX11Shader::get_const_buffer_desc() const
    { return &m_const_buffer_desc; }

    inline const D3D11_SUBRESOURCE_DATA* DX11Shader::get_subdata() const
    { return &m_constant_data; }

    inline Microsoft::WRL::ComPtr<ID3D11Buffer>& DX11Shader::get_const_buffer()
    { return m_constant_buffer; }
}

#endif // JOJ_DX11_SHADER_H
