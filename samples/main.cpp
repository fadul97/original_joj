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
#include "joj/renderer/dx11/index_buffer_dx11.h"
#include "joj/renderer/dx11/obj_const_buffer_dx11.h"

class MyApp : public joj::App
{
public:
    //joj::Cube geo = joj::Cube(3.0f, 3.0f, 3.0f);
    joj::Cylinder geo = joj::Cylinder(1.0f, 0.5f, 3.0f, 20, 10);
    //joj::GeoSphere geo = joj::GeoSphere(1.0f, 3);
    //joj::Grid geo = joj::Grid(100.0f, 100.0f, 20, 20);
    //joj::Quad geo = joj::Quad(3.0f, 1.0f);
    //joj::Sphere geo = joj::Sphere(1.0f, 40, 40, joj::Vector4(0, 1, 0, 1));
    joj::DX11Shader shader{};

    joj::DX11VertexBuffer vertex_buffer{};
    joj::DX11IndexBuffer index_buffer{};
    joj::DX11ObjectConstantBuffer objc_buffer{};

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
        // Control geometry rotation
        theta = DirectX::XM_PIDIV4;
        phi = DirectX::XM_PIDIV4;
        radius = 10.0f;

        // Get last mouse position
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

        objc_buffer.build(sizeof(DirectX::XMMATRIX), wvp);

        if FAILED(joj::Engine::renderer->get_device()->CreateBuffer(
            objc_buffer.get_const_buffer_desc(),
            objc_buffer.get_subdata(),
            objc_buffer.get_const_buffer().GetAddressOf()))
        {
            std::cout << "Failed to create object constant buffer.\n";
        }

        // Set constant buffer
        joj::Engine::renderer->get_device_context()->VSSetConstantBuffers(0, 1, objc_buffer.get_const_buffer().GetAddressOf());

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

        // ---------------------------------
        // INDICES AND INDEX BUFFERS
        // ---------------------------------

        // Build index buffer and set initial data
        index_buffer.build(sizeof(u32)* geo.get_index_count(), geo.get_index_data());

        // 3) Create buffer with ID3D11Device::CreateBuffer
        if (FAILED(joj::Engine::renderer->get_device()->CreateBuffer(
            index_buffer.get_buffer_desc(),
            index_buffer.get_subdata(),
            index_buffer.get_buffer().GetAddressOf())))
        {
            std::cout << "Failed to create index buffer.\n";
        }

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
            // Every pixel equals 1/4 degree
            f32 dx = DirectX::XMConvertToRadians(0.4f * (xmouse - last_xmouse));
            f32 dy = DirectX::XMConvertToRadians(0.4f * (ymouse - last_ymouse));

            // Update angles based on mouse offset to orbit camera around geometry
            theta += dx;
            phi += dy;

            // Restricts the angle of phi ]0-180[ degrees
            phi = phi < 0.1f ? 0.1f : (phi > (DirectX::XM_PI - 0.1f) ? DirectX::XM_PI - 0.1f : phi);
        }
        else if (joj::Engine::platform_manager->is_button_down(joj::BUTTON_RIGHT))
        {
            // Every pixel equals 0.5 unities
            f32 dx = 0.05f * (xmouse - last_xmouse);
            f32 dy = 0.05f * (ymouse - last_ymouse);

            // Update camera radius based on mouse offset
            radius += dx - dy;

            // Restricts radius (3 to 15 unities)
            radius = radius < 3.0f ? 3.0f : (radius > 15.0f ? 15.0f : radius);
        }

        last_xmouse = xmouse;
        last_ymouse = ymouse;

        // Converts spherical coordinates to Cartesian coordinates
        f32 x = radius * sinf(phi) * cosf(theta);
        f32 z = radius * sinf(phi) * sinf(theta);
        f32 y = radius * cosf(phi);

        // Constructs the Camera Matrix (View Matrix)
        DirectX::XMVECTOR pos = DirectX::XMVectorSet(x, y, z, 1.0f);
        DirectX::XMVECTOR target = DirectX::XMVectorZero();
        DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
        DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(pos, target, up);
        XMStoreFloat4x4(&View, view);

        // Constructs Combined Matrix (World x View x Proj)
        DirectX::XMMATRIX world = XMLoadFloat4x4(&World);
        DirectX::XMMATRIX proj = XMLoadFloat4x4(&Proj);
        DirectX::XMMATRIX WorldViewProj = world * view * proj;

        // Update constant buffer with combined matrix (Word-View-Projection Matrix)
        objc_buffer.set_matrix(WorldViewProj, joj::Engine::renderer->get_device_context());
    }

    void draw()
    {
        joj::Engine::renderer->clear();

        UINT stride = sizeof(joj::Vertex);
        UINT offset = 0;

        // Bind Vertex Buffer to an input slot of the device
        joj::Engine::renderer->get_device_context()->IASetVertexBuffers(0, 1, vertex_buffer.get_buffer().GetAddressOf(), &stride, &offset);
        
        // Bind index buffer to the pipeline
        joj::Engine::renderer->get_device_context()->IASetIndexBuffer(index_buffer.get_buffer().Get(), DXGI_FORMAT_R32_UINT, 0);

        // Bind Vertex and Pixel Shaders
        joj::Engine::renderer->get_device_context()->VSSetShader(shader.get_vertex_shader(), nullptr, 0);
        joj::Engine::renderer->get_device_context()->PSSetShader(shader.get_pixel_shader(), nullptr, 0);

        // Bind constant buffer
        joj::Engine::renderer->get_device_context()->VSSetConstantBuffers(0, 1, objc_buffer.get_const_buffer().GetAddressOf());

        // Draw
        joj::Engine::renderer->get_device_context()->DrawIndexedInstanced(geo.get_index_count(), 1, 0, 0, 0);
    }

    void shutdown()
    {
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
