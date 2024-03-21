#include "jmath.h"
#include "joj/engine.h"
#include "joj/app.h"

#include <iostream>
#include "joj/resources/geometry/cube.h"
#include "joj/resources/geometry/cylinder.h"
#include "joj/resources/geometry/geo_sphere.h"
#include "joj/resources/geometry/grid.h"
#include "joj/resources/geometry/quad.h"
#include "joj/resources/geometry/sphere.h"
#include "joj/systems/camera.h"
#include "matrix4.h"

class MyApp : public joj::App
{
public:
    //joj::Cube geo = joj::Cube(3.0f, 3.0f, 3.0f);
    //joj::Cylinder geo = joj::Cylinder(1.0f, 0.5f, 3.0f, 20, 10);
    //joj::GeoSphere geo = joj::GeoSphere(1.0f, 3);
    joj::Grid geo = joj::Grid(10.0f, 800.0f, 10, 20);
    //joj::Quad geo = joj::Quad(3.0f, 1.0f);
    //joj::Sphere geo = joj::Sphere(1.0f, 40, 40, joj::Vector4(0, 1, 0, 1));

    joj::Matrix4 World = {};
    joj::Matrix4 View = {};
    joj::Matrix4 Proj = {};

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
        if (joj::Engine::platform_manager->is_key_down(joj::KEY_W))
            camera.process_keyboard(joj::CameraMovement::FORWARD, dt);

        if (joj::Engine::platform_manager->is_key_down(joj::KEY_S))
            camera.process_keyboard(joj::CameraMovement::BACKWARD, dt);

        if (joj::Engine::platform_manager->is_key_down(joj::KEY_A))
            camera.process_keyboard(joj::CameraMovement::LEFT, dt);

        if (joj::Engine::platform_manager->is_key_down(joj::KEY_D))
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
        World = joj::Matrix4();

        // View Matrix
        View = camera.get_view_mat();
            
        // Projection Matrix
        Proj = joj::perspective_lh(joj::to_radians(45.0f), joj::Engine::platform_manager->get_window()->get_aspect_ratio(), 1.0f, 100.0f);
        

        // Word-View-Projection Matrix
        joj::Matrix4 wvp = World * View * Proj;

        mouse_callback(joj::Engine::platform_manager->get_xmouse(), joj::Engine::platform_manager->get_ymouse());
        
        centerX = joj::Engine::platform_manager->get_window()->get_xcenter();
        centerY = joj::Engine::platform_manager->get_window()->get_ycenter();

        joj::Engine::platform_manager->get_window()->hide_cursor(true);

        cmouseX = joj::Engine::platform_manager->get_xmouse();
        cmouseY = joj::Engine::platform_manager->get_ymouse();

        camera.m_movement_speed = 20.0f;
    }

    void update(f32 dt)
    {
        if (joj::Engine::platform_manager->is_key_pressed(joj::KEY_ESCAPE))
            joj::Engine::close();

        if (joj::Engine::platform_manager->is_key_pressed(joj::KEY_TAB))
        {
            firstPerson = !firstPerson;
            hideCursor = !hideCursor;
        }

        if (firstPerson)
        {
            process_input_for_camera(dt);
        }

        
        // Constructs the Camera Matrix (View Matrix)
        View = camera.get_view_mat();

        // Constructs Combined Matrix (World x View x Proj)
        joj::Matrix4 WorldViewProj = World * View * Proj;
    }

    void draw()
    {
        joj::Engine::renderer->clear(1, 1, 0, 0);


        joj::Engine::platform_manager->swap_buffers();
    }

    void shutdown()
    {
    }
};
