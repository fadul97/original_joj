#include "joj/engine.h"
#include "joj/app.h"
#include <stdio.h>

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
#include "joj/systems/camera.h"

class DX11App : public joj::App
{
public:
    //joj::Cube geo = joj::Cube(3.0f, 3.0f, 3.0f);
    //joj::Cylinder geo = joj::Cylinder(1.0f, 0.5f, 3.0f, 20, 10);
    //joj::GeoSphere geo = joj::GeoSphere(1.0f, 3);
    joj::Grid geo = joj::Grid(10.0f, 800.0f, 10, 20);
    //joj::Quad geo = joj::Quad(3.0f, 1.0f);
    //joj::Sphere geo = joj::Sphere(1.0f, 40, 40, joj::Vector4(0, 1, 0, 1));
    joj::DX11Shader shader{};

    joj::DX11VertexBuffer vertex_buffer{};
    joj::DX11IndexBuffer index_buffer{};
    joj::DX11ObjectConstantBuffer objc_buffer{};

    DirectX::XMFLOAT4X4 World = {};
    DirectX::XMFLOAT4X4 View = {};
    DirectX::XMFLOAT4X4 Proj = {};

    joj::Camera camera = joj::Camera{ joj::Vector3{ 0.0f, 0.0f, -6.0f } };

    float lastX = 800 / 2.0f;
    float lastY = 600 / 2.0f;
    b8 firstMouse = true;
    b8 firstPerson = true;
    b8 hideCursor = false;
    i32 centerX = 400;
    i32 centerY = 300;
    i32 cmouseX = 0;
    i32 cmouseY = 0;

    void process_input_for_camera(f32 dt)
    {
        if (joj::Engine::platform_manager->is_key_down('W'))
            camera.process_keyboard(joj::CameraMovement::FORWARD, dt);

        if (joj::Engine::platform_manager->is_key_down('S'))
            camera.process_keyboard(joj::CameraMovement::BACKWARD, dt);

        if (joj::Engine::platform_manager->is_key_down('A'))
            camera.process_keyboard(joj::CameraMovement::LEFT, dt);

        if (joj::Engine::platform_manager->is_key_down('D'))
            camera.process_keyboard(joj::CameraMovement::RIGHT, dt);
    }

    void mouse_callback(f64 xposIn, f64 yposIn)
    {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera.process_mouse_movement(xoffset, yoffset);
    }

    void init()
    {
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
            1.0f, 1000.0f));

        // World Matrix
        DirectX::XMMATRIX S = DirectX::XMMatrixScaling(30.0f, 30.0f, 30.0f);
        DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(30));
        DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(-30));
        DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(0, 0, 0);
        DirectX::XMMATRIX w = S * Ry * Rx * T;

        // View Matrix
        DirectX::XMMATRIX v = camera.get_view_mat();

        // Projection Matrix
        DirectX::XMMATRIX p = DirectX::XMMatrixPerspectiveFovLH(
            DirectX::XMConvertToRadians(camera.m_zoom),
            joj::Engine::platform_manager->get_window()->get_aspect_ratio(),
            1.0f, 100.0f);

        // Word-View-Projection Matrix
        DirectX::XMMATRIX wvp = w * v * p;

        // ---------------------------------
        // CONSTANT BUFFER
        // ---------------------------------

        objc_buffer.build(sizeof(DirectX::XMMATRIX), wvp);

        if FAILED(joj::Engine::dx11_renderer->get_device()->CreateBuffer(
            objc_buffer.get_const_buffer_desc(),
            objc_buffer.get_subdata(),
            objc_buffer.get_const_buffer().GetAddressOf()))
        {
            std::cout << "Failed to create object constant buffer.\n";
        }

        // Set constant buffer
        joj::Engine::dx11_renderer->get_device_context()->VSSetConstantBuffers(0, 1, objc_buffer.get_const_buffer().GetAddressOf());

        // ---------------------------------
        // VERTEX BUFFERS
        // ---------------------------------
        
        // Build vertex buffer and set initial data
        vertex_buffer.build(sizeof(joj::Vertex) * geo.get_vertex_count(), geo.get_vertex_data());

        // Create buffer with ID3D11Device::CreateBuffer
        if FAILED(joj::Engine::dx11_renderer->get_device()->CreateBuffer(
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
        if (FAILED(joj::Engine::dx11_renderer->get_device()->CreateBuffer(
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
        if (!shader.compile_shaders(vertex_path, pixel_path, joj::Engine::dx11_renderer->get_device()))
            std::cout << "Failed to compile shaders.\n";
    
        // ---------------------------------
        // INPUT ASSEMBLER
        // ---------------------------------
        
        // Bind input layout to the Input Assembler Stage
        joj::Engine::dx11_renderer->get_device_context()->IASetInputLayout(shader.get_input_layout());

        // Tell how Direct3D will form geometric primitives from vertex data
        joj::Engine::dx11_renderer->set_primitive_topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        mouse_callback(joj::Engine::platform_manager->get_xmouse(), joj::Engine::platform_manager->get_ymouse());
        
        centerX = joj::Engine::platform_manager->get_xcenter();
        centerY = joj::Engine::platform_manager->get_ycenter();

        joj::Engine::platform_manager->hide_cursor(true);

        cmouseX = joj::Engine::platform_manager->get_xmouse();
        cmouseY = joj::Engine::platform_manager->get_ymouse();

        camera.m_movement_speed = 20.0f;
    }

    void update(f32 dt)
    {
        if (joj::Engine::platform_manager->is_key_pressed(joj::KEY_ESCAPE))
            joj::Engine::close();

        if (joj::Engine::platform_manager->is_key_pressed('O'))
            printf("OI");

        if (joj::Engine::platform_manager->is_key_pressed(joj::KEY_TAB))
        {
            firstPerson = !firstPerson;
            hideCursor = !hideCursor;
            ShowCursor(hideCursor);
        }

        if (firstPerson)
        {
            // Now read the mouse position
            joj::Point cursor_position = joj::Engine::platform_manager->get_cursor_position();  // Get the current cursor position
            // Rotate freely inside window
            joj::Engine::platform_manager->set_cursor_position(centerX, centerY);

            // Calculate the mouse movement
            int xoffset = cursor_position.x - centerX;
            int yoffset = centerY - cursor_position.y;

            //mouse_callback(JojEngine::Engine::pm->get_xmouse(), JojEngine::Engine::pm->get_ymouse());
            camera.process_mouse_movement(xoffset, yoffset);
            process_input_for_camera(dt);
        }

        // Transformations
        DirectX::XMMATRIX world = XMLoadFloat4x4(&World);
        
        // Constructs the Camera Matrix (View Matrix)
        DirectX::XMMATRIX view = camera.get_view_mat();
        XMStoreFloat4x4(&View, view);

        // Projection Matrix
        DirectX::XMMATRIX proj = XMLoadFloat4x4(&Proj);

        // Constructs Combined Matrix (World x View x Proj)
        DirectX::XMMATRIX WorldViewProj = world * view * proj;

        // Update constant buffer with combined matrix (Word-View-Projection Matrix)
        objc_buffer.set_matrix(WorldViewProj, joj::Engine::dx11_renderer->get_device_context());
    }

    void draw()
    {
        joj::Engine::dx11_renderer->clear();

        UINT stride = sizeof(joj::Vertex);
        UINT offset = 0;

        // Bind Vertex Buffer to an input slot of the device
        joj::Engine::dx11_renderer->get_device_context()->IASetVertexBuffers(0, 1, vertex_buffer.get_buffer().GetAddressOf(), &stride, &offset);
        
        // Bind index buffer to the pipeline
        joj::Engine::dx11_renderer->get_device_context()->IASetIndexBuffer(index_buffer.get_buffer().Get(), DXGI_FORMAT_R32_UINT, 0);

        // Bind Vertex and Pixel Shaders
        joj::Engine::dx11_renderer->get_device_context()->VSSetShader(shader.get_vertex_shader(), nullptr, 0);
        joj::Engine::dx11_renderer->get_device_context()->PSSetShader(shader.get_pixel_shader(), nullptr, 0);

        // Bind constant buffer
        joj::Engine::dx11_renderer->get_device_context()->VSSetConstantBuffers(0, 1, objc_buffer.get_const_buffer().GetAddressOf());

        // Draw
        joj::Engine::dx11_renderer->get_device_context()->DrawIndexedInstanced(geo.get_index_count(), 1, 0, 0, 0);

        joj::Engine::dx11_renderer->swap_buffers();
    }

    void shutdown()
    {
    }
};
