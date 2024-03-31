#include "joj/engine.h"
#include "joj/app.h"
#include <stdio.h>

#include "joj/resources/geometry/cylinder.h"
#include "joj/renderer/opengl/shader_gl.h"
#include "joj/math/jmath.h"
#include "joj/systems/camera.h"

class GLApp : public joj::App
{
public:
    unsigned int VBO, VAO, EBO;
    joj::Cylinder geo = joj::Cylinder(1.0f, 0.5f, 3.0f, 20, 10);
    joj::GLShader shader;

    joj::Camera camera = joj::Camera{ joj::Vector3{ 0.0f, 0.0f, -6.0f } };

    DirectX::XMFLOAT4X4 World = {};
    DirectX::XMFLOAT4X4 View = {};
    DirectX::XMFLOAT4X4 Proj = {};

    // Movement
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
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, geo.get_vertex_count() * sizeof(joj::Vertex), geo.get_vertex_data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, geo.get_index_count() * sizeof(unsigned int), geo.get_index_data(), GL_STATIC_DRAW);

        // Specify the layout of the vertex(pos) data
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(joj::Vertex), (void*)0);

        // Specify the layout of the vertex(color) data
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(joj::Vertex), (GLvoid*)(3 * sizeof(f32)));

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);

        //shader.compile_shaders("../../../../samples/shaders/vertex.glsl", "../../../../samples/shaders/frag.glsl");
        shader = joj::GLShader{ "../../../../samples/shaders/vertex.glsl", "../../../../samples/shaders/frag.glsl" };
        
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

        mouse_callback(joj::Engine::platform_manager->get_xmouse(), joj::Engine::platform_manager->get_ymouse());

        centerX = joj::Engine::platform_manager->get_xcenter();
        centerY = joj::Engine::platform_manager->get_ycenter();

        joj::Engine::platform_manager->hide_cursor(true);

        cmouseX = joj::Engine::platform_manager->get_xmouse();
        cmouseY = joj::Engine::platform_manager->get_ymouse();

        camera.m_movement_speed = 20.0f;
    
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    void update(f32 dt)
    {
        if (joj::Engine::platform_manager->is_key_pressed(joj::KEY_ESCAPE))
            joj::Engine::close();

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

        shader.use();
        shader.set_mat4("transform", WorldViewProj);
    }

    void draw()
    {
        //joj::Engine::gl_renderer->clear(0, 1, 1, 1);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawElements(GL_TRIANGLES, geo.get_index_count(), GL_UNSIGNED_INT, 0);

        SwapBuffers(joj::Engine::platform_manager->get_window()->get_device_context());
    }

    void shutdown()
    {
    }
};
