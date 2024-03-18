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
#include "joj/renderer/dx11/shader_dx11.h"
#include "joj/renderer/dx11/vertex_buffer_dx11.h"

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
    joj::Sphere geo = joj::Sphere(1.0f, 40, 40, joj::Vector4(0, 1, 0, 1));
    joj::DX11Shader shader{};

    joj::DX11VertexBuffer vertex_buffer{};
    ID3D11Buffer* index_buffer = nullptr;

    ID3D11Buffer* constant_buffer = nullptr;
    D3D11_SUBRESOURCE_DATA constant_data = { 0 };
    D3D11_BUFFER_DESC const_buffer_desc = { 0 };

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
        
        // Build vertex buffer and set initial data
        vertex_buffer.build(sizeof(joj::Vertex) * geo.get_vertex_count(), geo.get_vertex_data());

        // Create buffer with ID3D11Device::CreateBuffer
        if FAILED(joj::Engine::renderer->get_device()->CreateBuffer(
            vertex_buffer.get_buffer_desc(),
            vertex_buffer.get_subdata(),
            vertex_buffer.get_buffer().GetAddressOf()))
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

        LPCWSTR vertex_path = L"../../../../samples/shaders/vert.hlsl";
        LPCWSTR pixel_path = L"../../../../samples/shaders/pixel.hlsl";
        if (!shader.compile_shaders(vertex_path, pixel_path, joj::Engine::renderer->get_device()))
            std::cout << "Failed to compile shaders.\n";
    
        // ---------------------------------
        // INPUT ASSEMBLER
        // ---------------------------------
        
        // Bind input layout to the Input Assembler Stage
        joj::Engine::renderer->get_device_context()->IASetInputLayout(shader.get_input_layout());

        // Tell how Direct3D will form geometric primitives from vertex data
        joj::Engine::renderer->set_primitive_topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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
        joj::Engine::renderer->get_device_context()->IASetVertexBuffers(0, 1, vertex_buffer.get_buffer().GetAddressOf(), &stride, &offset);
        
        // Bind index buffer to the pipeline
        joj::Engine::renderer->get_device_context()->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);

        // Bind Vertex and Pixel Shaders
        joj::Engine::renderer->get_device_context()->VSSetShader(shader.get_vertex_shader(), nullptr, 0);
        joj::Engine::renderer->get_device_context()->PSSetShader(shader.get_pixel_shader(), nullptr, 0);

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

        // Release Index buffer;
        if (index_buffer)
            index_buffer->Release();
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
