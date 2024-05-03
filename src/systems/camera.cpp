#include "systems/camera.h"

joj::Camera::Camera(Vector3 pos, Vector3 up, f32 yaw, f32 pitch)
    : m_target(Vector3{ 0.0f, 0.0f, 0.0f }), m_movement_speed(SPEED), m_mouse_sensitivity(SENSITIVITY), m_zoom(ZOOM)
{
    m_position = pos;
    m_world_up = up;
    m_yaw = yaw;
    m_pitch = pitch;
    update_camera_vectors();
}

joj::Camera::~Camera()
{

}

void joj::Camera::process_keyboard(CameraMovement direction, f32 dt)
{

}

void joj::Camera::process_mouse_movement(f32 xoffset, f32 yoffset, b8 constrain_pitch)
{
    xoffset *= m_mouse_sensitivity;
    yoffset *= m_mouse_sensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    if (constrain_pitch)
    {
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;
    }

    update_camera_vectors();
}

void joj::Camera::process_mouse_scroll(f32 yoffset)
{
    m_zoom -= yoffset;
    if (m_zoom < 1.0f)
        m_zoom = 1.0f;
    if (m_zoom > 45.0f)
        m_zoom = 45.0f;
}

void joj::Camera::update_camera_vectors()
{

}
