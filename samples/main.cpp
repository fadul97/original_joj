#include "joj/engine.h"
#include "joj/app.h"

#include <DirectXMath.h>
#include <DirectXColors.h>
#include <iostream>
#include <dxgi.h>
#include <d3dcompiler.h>
#include "joj/resources/geometry/cube.h"
#include "joj/resources/geometry/cylinder.h"
#include "joj/resources/geometry/geo_sphere.h"
#include "joj/resources/geometry/grid.h"
#include "joj/resources/geometry/quad.h"
#include "joj/resources/geometry/sphere.h"

/*
struct Vertex1
{
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT4 color;
};
*/

struct ObjectConstant
{
    DirectX::XMFLOAT4X4 world_view_proj =
    {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
};

class MyApp : public joj::App
{
public:
    //joj::Cube geo = joj::Cube(3.0f, 3.0f, 3.0f);
    //joj::Cylinder geo = joj::Cylinder(1.0f, 0.5f, 3.0f, 20, 10);
    //joj::GeoSphere geo = joj::GeoSphere(1.0f, 3);
    //joj::Grid geo = joj::Grid(100.0f, 100.0f, 20, 20);
    //joj::Quad geo = joj::Quad(3.0f, 1.0f);
    joj::Sphere geo = joj::Sphere(1.0f, 40, 40, joj::Vector4(0, 1, 1, 1));

    ID3D11Buffer* vertex_buffer = nullptr;
    ID3D11Buffer* index_buffer = nullptr;

    ID3D11Buffer* constant_buffer = nullptr;
    D3D11_SUBRESOURCE_DATA constant_data = { 0 };
    D3D11_BUFFER_DESC const_buffer_desc = { 0 };

    ID3D11VertexShader* vertex_shader = nullptr;
    ID3D11PixelShader* pixel_shader = nullptr;

    DirectX::XMFLOAT4X4 World = {};
    DirectX::XMFLOAT4X4 View = {};
    DirectX::XMFLOAT4X4 Proj = {};

    f32 theta = 0;
    f32 phi = 0;
    f32 radius = 0;

    f32 last_xmouse = 0;
    f32 last_ymouse = 0;

    void init()
    {
        // controla rotação do cubo
        theta = DirectX::XM_PIDIV4;
        phi = DirectX::XM_PIDIV4;
        radius = 10.0f;

        // pega última posição do mouse
        last_xmouse = (f32)joj::Engine::platform_manager->get_xmouse();
        last_ymouse = (f32)joj::Engine::platform_manager->get_ymouse();

        World = View =
        {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

        XMStoreFloat4x4(&Proj, DirectX::XMMatrixPerspectiveFovLH(
            DirectX::XMConvertToRadians(45.0f),
            joj::Engine::platform_manager->get_window()->get_aspect_ratio(),
            1.0f, 100.0f));

        // World Matrix
        DirectX::XMMATRIX S = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
        DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(30));
        DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(-30));
        DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(0, 0, 0);
        DirectX::XMMATRIX w = S * Ry * Rx * T;

        // View Matrix
        DirectX::XMVECTOR pos = DirectX::XMVectorSet(0, 0, -6, 1);
        DirectX::XMVECTOR target = DirectX::XMVectorZero();
        DirectX::XMVECTOR up = DirectX::XMVectorSet(0, 1, 0, 0);
        DirectX::XMMATRIX v = DirectX::XMMatrixLookAtLH(pos, target, up);

        // Projection Matrix
        DirectX::XMMATRIX p = DirectX::XMMatrixPerspectiveFovLH(
            DirectX::XMConvertToRadians(45),
            joj::Engine::platform_manager->get_window()->get_aspect_ratio(),
            1.0f, 100.0f);

        // Word-View-Projection Matrix
        DirectX::XMMATRIX wvp = w * v * p;

        // ---------------------------------
        // CONSTANT BUFFER
        // ---------------------------------

        const_buffer_desc.ByteWidth = sizeof(DirectX::XMMATRIX);
        const_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
        const_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        const_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        DirectX::XMMATRIX world_view_proj = DirectX::XMMatrixTranspose(wvp);
        constant_data.pSysMem = &world_view_proj;

        joj::Engine::renderer->get_device()->CreateBuffer(&const_buffer_desc, &constant_data, &constant_buffer);

        joj::Engine::renderer->get_device_context()->VSSetConstantBuffers(0, 1, &constant_buffer);

        // ---------------------------------
        // VERTEX BUFFERS
        // ---------------------------------
        
        // 1) Buffer to store vertices, describe how data will be accessed and where it will be bound to the rendering pipeline
        D3D11_BUFFER_DESC vbd;
        vbd.Usage = D3D11_USAGE_IMMUTABLE;
        vbd.ByteWidth = sizeof(joj::Vertex) * geo.get_vertex_count();
        vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vbd.CPUAccessFlags = 0;
        vbd.MiscFlags = 0;
        vbd.StructureByteStride = 0;

        // 2) Strcture to specify data we want to initialize the buffer contents with
        D3D11_SUBRESOURCE_DATA vert_init_data{ geo.get_vertex_data(), 0, 0 };

        // 3) Create buffer with ID3D11Device::CreateBuffer
        if FAILED(joj::Engine::renderer->get_device()->CreateBuffer(&vbd, &vert_init_data, &vertex_buffer))
        {
            std::cout << "Failed to create vertex buffer.\n";
        }

        /*
        // 4) Bind vertex buffer to input slot of the device when drawing...
        u32 stride = sizeof(Vertex1);
        u32 offset = 0;
        joj::Engine::renderer->get_device_context()->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
        */


        // ---------------------------------
        // INDICES AND INDEX BUFFERS
        // ---------------------------------

        // 1) Buffer to store indices, describe how data will be accessed and where it will be bound to the rendering pipeline
        D3D11_BUFFER_DESC ibd;
        ibd.Usage = D3D11_USAGE_IMMUTABLE;
        ibd.ByteWidth = sizeof(u32) * geo.get_index_count();
        ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        ibd.CPUAccessFlags = 0;
        ibd.MiscFlags = 0;
        ibd.StructureByteStride = 0;

        // 2) Strcture to specify data we want to initialize the buffer contents with
        D3D11_SUBRESOURCE_DATA index_init_data;
        index_init_data.pSysMem = geo.get_index_data();

        // 3) Create buffer with ID3D11Device::CreateBuffer
        if (FAILED(joj::Engine::renderer->get_device()->CreateBuffer(&ibd, &index_init_data, &index_buffer)))
        {
            std::cout << "Failed to create index buffer.\n";
        }

        /*
        // 4) Bind index buffer to input slot of the device when drawing...
        joj::Engine::renderer->get_device_context()->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);
        */

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
        if FAILED(D3DCompileFromFile(filepath, nullptr, nullptr, "main", "vs_5_0", shader_flags, NULL, &vs_blob, &compilation_msgs))
        {
            wprintf(L"Failed to compile vertex shader from path '%s'\n", filepath);
        }

        if FAILED(joj::Engine::renderer->get_device()->CreateVertexShader(vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), nullptr, &vertex_shader))
        {
            std::cout << "Failed to create vertex shader.\n";
        }

        ID3DBlob* compile_errors_blob;
        ID3DBlob* ps_blob = nullptr;

        filepath = L"../../../../samples/shaders/pixel.hlsl";
        if FAILED(D3DCompileFromFile(filepath, nullptr, nullptr, "main", "ps_5_0", shader_flags, NULL, &ps_blob, &compile_errors_blob))
            MessageBoxA(nullptr, "Failed to compile Pixel Shader.", 0, 0);

        if FAILED(joj::Engine::renderer->get_device()->CreatePixelShader(ps_blob->GetBufferPointer(), ps_blob->GetBufferSize(), nullptr, &pixel_shader))
            MessageBoxA(nullptr, "Failed to create Vertex Shader.", 0, 0);
    
        // ---------------------------------
        // INPUT ASSEMBLER
        // ---------------------------------
        
        // Create input layout
        ID3D11InputLayout* input_layout = nullptr;

        // Create the vertex input layout.
        D3D11_INPUT_ELEMENT_DESC input_desc[2] =
        {
            { "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }				// 3 'floats' x 4 bytes = 12 bytes
        };

        if FAILED(joj::Engine::renderer->get_device()->CreateInputLayout(input_desc, ARRAYSIZE(input_desc), vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), &input_layout))
        {
            MessageBoxA(nullptr, "Failed to create Input Layout.", 0, 0);
        }

        // Bind input layout to the Input Assembler Stage
        joj::Engine::renderer->get_device_context()->IASetInputLayout(input_layout);

        // Tell how Direct3D will form geometric primitives from vertex data
        joj::Engine::renderer->set_primitive_topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // Relase Direct3D resources
        vs_blob->Release();
        ps_blob->Release();
    }

    void update(f32 dt)
    {
        using namespace joj;

        f32 xmouse = (f32)joj::Engine::platform_manager->get_xmouse();
        f32 ymouse = (f32)joj::Engine::platform_manager->get_ymouse();

        if (joj::Engine::platform_manager->is_button_down(joj::BUTTON_LEFT))
        {
            // cada pixel corresponde a 1/4 de grau
            f32 dx = DirectX::XMConvertToRadians(0.4f * (xmouse - last_xmouse));
            f32 dy = DirectX::XMConvertToRadians(0.4f * (ymouse - last_ymouse));

            // atualiza ângulos com base no deslocamento do mouse 
            // para orbitar a câmera ao redor da caixa
            theta += dx;
            phi += dy;

            // restringe o ângulo de phi ]0-180[ graus
            phi = phi < 0.1f ? 0.1f : (phi > (DirectX::XM_PI - 0.1f) ? DirectX::XM_PI - 0.1f : phi);
        }
        else if (joj::Engine::platform_manager->is_button_down(joj::BUTTON_RIGHT))
        {
            // cada pixel corresponde a 0.05 unidades
            f32 dx = 0.05f * (xmouse - last_xmouse);
            f32 dy = 0.05f * (ymouse - last_ymouse);

            // atualiza o raio da câmera com base no deslocamento do mouse 
            radius += dx - dy;

            // restringe o raio (3 a 15 unidades)
            radius = radius < 3.0f ? 3.0f : (radius > 15.0f ? 15.0f : radius);
        }

        last_xmouse = xmouse;
        last_ymouse = ymouse;

        // converte coordenadas esféricas para cartesianas
        f32 x = radius * sinf(phi) * cosf(theta);
        f32 z = radius * sinf(phi) * sinf(theta);
        f32 y = radius * cosf(phi);

        // constrói a matriz da câmera (view matrix)
        DirectX::XMVECTOR pos = DirectX::XMVectorSet(x, y, z, 1.0f);
        DirectX::XMVECTOR target = DirectX::XMVectorZero();
        DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
        DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(pos, target, up);
        XMStoreFloat4x4(&View, view);

        // constrói matriz combinada (world x view x proj)
        DirectX::XMMATRIX world = XMLoadFloat4x4(&World);
        DirectX::XMMATRIX proj = XMLoadFloat4x4(&Proj);
        DirectX::XMMATRIX WorldViewProj = world * view * proj;

        // Update constant buffer with combined matrix (Word-View-Projection Matrix)
        ObjectConstant objc;
        XMStoreFloat4x4(&objc.world_view_proj, DirectX::XMMatrixTranspose(WorldViewProj));
        constant_data.pSysMem = &objc.world_view_proj;

        // Get a pointer to the constant buffer data.
        D3D11_MAPPED_SUBRESOURCE mapped_buffer = {};
        joj::Engine::renderer->get_device_context()->Map(constant_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_buffer);

        // Copy the new data to the constant buffer data.
        memcpy(mapped_buffer.pData, &objc, sizeof(ObjectConstant));

        // Release the pointer to the constant buffer data.
        joj::Engine::renderer->get_device_context()->Unmap(constant_buffer, 0);
    }

    void draw()
    {
        joj::Engine::renderer->clear();

        UINT stride = sizeof(joj::Vertex);
        UINT offset = 0;

        // Bind Vertex Buffer to an input slot of the device
        joj::Engine::renderer->get_device_context()->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
        
        // Bind index buffer to the pipeline
        joj::Engine::renderer->get_device_context()->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);

        // Bind Vertex and Pixel Shaders
        joj::Engine::renderer->get_device_context()->VSSetShader(vertex_shader, nullptr, 0);
        joj::Engine::renderer->get_device_context()->PSSetShader(pixel_shader, nullptr, 0);

        // Bind constant buffer
        joj::Engine::renderer->get_device_context()->VSSetConstantBuffers(0, 1, &constant_buffer);

        // Draw
        joj::Engine::renderer->get_device_context()->DrawIndexedInstanced(geo.get_index_count(), 1, 0, 0, 0);
    }

    void shutdown()
    {
        // Release constant buffer
        if (constant_buffer)
            constant_buffer->Release();

        // Release Buffer resource
        if (vertex_buffer)
            vertex_buffer->Release();

        // Release Index buffer;
        if (index_buffer)
            index_buffer->Release();

        // Release Vertex Shader
        if (vertex_shader)
            vertex_shader->Release();

        // Release Pixel Shader
        if (pixel_shader)
            pixel_shader->Release();
    }
};

int main()
{
    joj::Engine* engine = new joj::Engine();
    if (engine->run(new MyApp()) != joj::ErrorCode::OK)
        return -1;

    delete engine;
    return 0;
}
