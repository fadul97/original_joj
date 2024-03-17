#include "renderer/dx11/shader_dx11.h"

#include <iostream>

joj::DX11Shader::DX11Shader()
{
    m_vertex_shader = nullptr;
    m_pixel_shader = nullptr;
    m_layout = nullptr;
    m_matrix_buffer = nullptr;
}

joj::DX11Shader::DX11Shader(LPCWSTR vertex_path, LPCWSTR pixel_path, Microsoft::WRL::ComPtr<ID3D11Device>& device)
{
    m_vertex_shader = nullptr;
    m_pixel_shader = nullptr;
    m_layout = nullptr;
    m_matrix_buffer = nullptr;

    // ---------------------------------
    // SHADERS
    // ---------------------------------

    DWORD shader_flags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    shader_flags |= D3D10_SHADER_DEBUG;
    shader_flags |= D3D10_SHADER_SKIP_OPTIMIZATION;
    std::cout << "[DEBUG MODE]" << std::endl;
#endif

    ID3DBlob* vs_blob = nullptr;
    ID3D10Blob* compilation_msgs = 0;

    LPCWSTR filepath = L"../../../../samples/shaders/vert.hlsl";
    if FAILED(D3DCompileFromFile(vertex_path, nullptr, nullptr, "main", "vs_5_0", shader_flags, NULL, &vs_blob, &compilation_msgs))
    {
        wprintf(L"Failed to compile vertex shader from path '%s'\n", filepath);
    }

    if FAILED(device->CreateVertexShader(vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), nullptr, &m_vertex_shader))
    {
        std::cout << "Failed to create vertex shader.\n";
    }

    ID3DBlob* compile_errors_blob;
    ID3DBlob* ps_blob = nullptr;

    filepath = L"../../../../samples/shaders/pixel.hlsl";
    if FAILED(D3DCompileFromFile(pixel_path, nullptr, nullptr, "main", "ps_5_0", shader_flags, NULL, &ps_blob, &compile_errors_blob))
        MessageBoxA(nullptr, "Failed to compile Pixel Shader.", 0, 0);

    if FAILED(device->CreatePixelShader(ps_blob->GetBufferPointer(), ps_blob->GetBufferSize(), nullptr, &m_pixel_shader))
        MessageBoxA(nullptr, "Failed to create Vertex Shader.", 0, 0);

    // ---------------------------------
    // INPUT ASSEMBLER
    // ---------------------------------

    // Create the vertex input layout.
    D3D11_INPUT_ELEMENT_DESC input_desc[2] =
    {
        { "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }				// 3 'floats' x 4 bytes = 12 bytes
    };

    if FAILED(device->CreateInputLayout(input_desc, ARRAYSIZE(input_desc), vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), &m_layout))
        MessageBoxA(nullptr, "Failed to create Input Layout.", 0, 0);

    // Relase Direct3D resources
    vs_blob->Release();
    ps_blob->Release();
}

joj::DX11Shader::~DX11Shader()
{
	// Release matrix constant buffer
	if (m_matrix_buffer)
	{
		m_matrix_buffer->Release();
		m_matrix_buffer = nullptr;
	}

	// Release layout
	if (m_layout)
	{
		m_layout->Release();
		m_layout = nullptr;
	}

	// Release pixel shader
	if (m_pixel_shader)
	{
		m_pixel_shader->Release();
		m_pixel_shader = nullptr;
	}

	// Release vertex shader
	if (m_vertex_shader)
	{
		m_vertex_shader->Release();
		m_vertex_shader = nullptr;
	}
}

b8 joj::DX11Shader::compile_shaders(LPCWSTR vertex_path, LPCWSTR pixel_path, Microsoft::WRL::ComPtr<ID3D11Device>& device)
{
    // ---------------------------------
    // SHADERS
    // ---------------------------------

    DWORD shader_flags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    shader_flags |= D3D10_SHADER_DEBUG;
    shader_flags |= D3D10_SHADER_SKIP_OPTIMIZATION;
    std::cout << "[DEBUG MODE]" << std::endl;
#endif

    ID3DBlob* vs_blob = nullptr;
    ID3D10Blob* compilation_msgs = 0;

    LPCWSTR filepath = L"../../../../samples/shaders/vert.hlsl";
    if FAILED(D3DCompileFromFile(vertex_path, nullptr, nullptr, "main", "vs_5_0", shader_flags, NULL, &vs_blob, &compilation_msgs))
    {
        wprintf(L"Failed to compile vertex shader from path '%s'\n", filepath);
        return false;
    }

    if FAILED(device->CreateVertexShader(vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), nullptr, &m_vertex_shader))
    {
        std::cout << "Failed to create vertex shader.\n";
        return false;
    }

    ID3DBlob* compile_errors_blob;
    ID3DBlob* ps_blob = nullptr;

    filepath = L"../../../../samples/shaders/pixel.hlsl";
    if FAILED(D3DCompileFromFile(pixel_path, nullptr, nullptr, "main", "ps_5_0", shader_flags, NULL, &ps_blob, &compile_errors_blob))
    {
        MessageBoxA(nullptr, "Failed to compile Pixel Shader.", 0, 0);
        return false;
    }

    if FAILED(device->CreatePixelShader(ps_blob->GetBufferPointer(), ps_blob->GetBufferSize(), nullptr, &m_pixel_shader))
    {
        MessageBoxA(nullptr, "Failed to create Vertex Shader.", 0, 0);
        return false;
    }

    // ---------------------------------
    // INPUT ASSEMBLER
    // ---------------------------------


    // Create the vertex input layout.
    D3D11_INPUT_ELEMENT_DESC input_desc[2] =
    {
        { "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }				// 3 'floats' x 4 bytes = 12 bytes
    };

    if FAILED(device->CreateInputLayout(input_desc, ARRAYSIZE(input_desc), vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), &m_layout))
    {
        MessageBoxA(nullptr, "Failed to create Input Layout.", 0, 0);
        return false;
    }

    // Relase Direct3D resources
    vs_blob->Release();
    ps_blob->Release();

    return true;
}

void joj::DX11Shader::use() const
{

}

void joj::DX11Shader::destroy()
{
    // Release matrix constant buffer
    if (m_matrix_buffer)
    {
        m_matrix_buffer->Release();
        m_matrix_buffer = nullptr;
    }

    // Release layout
    if (m_layout)
    {
        m_layout->Release();
        m_layout = nullptr;
    }

    // Release pixel shader
    if (m_pixel_shader)
    {
        m_pixel_shader->Release();
        m_pixel_shader = nullptr;
    }

    // Release vertex shader
    if (m_vertex_shader)
    {
        m_vertex_shader->Release();
        m_vertex_shader = nullptr;
    }
}