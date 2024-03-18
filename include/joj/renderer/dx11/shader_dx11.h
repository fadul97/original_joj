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
    class JAPI DX11Shader : public Shader
    {
    public:
        DX11Shader();
        DX11Shader(LPCWSTR vertex_path, LPCWSTR pixel_path, Microsoft::WRL::ComPtr<ID3D11Device>& device);
        ~DX11Shader();

        b8 compile_shaders(LPCWSTR vertex_path, LPCWSTR pixel_path, Microsoft::WRL::ComPtr<ID3D11Device>& device);
        void use() const override;
        void destroy() override;

        ID3D11VertexShader* get_vertex_shader();
        ID3D11PixelShader* get_pixel_shader();
        ID3D11InputLayout* get_input_layout();

    private:
        ID3D11VertexShader* m_vertex_shader;
        ID3D11PixelShader* m_pixel_shader;
        ID3D11InputLayout* m_layout;
    };

    inline ID3D11VertexShader* DX11Shader::get_vertex_shader()
    { return m_vertex_shader; }

    inline ID3D11PixelShader* DX11Shader::get_pixel_shader()
    { return m_pixel_shader; }

    inline ID3D11InputLayout* DX11Shader::get_input_layout()
    { return m_layout; }
}

#endif // JOJ_DX11_SHADER_H
