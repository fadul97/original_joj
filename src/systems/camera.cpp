#include "systems/camera.h"

joj::Camera::Camera(const Vector3 pos, const Vector3 up, const f32 yaw, const f32 pitch)
    : m_target(Vector3{ 0.0f, 0.0f, -1.0f }), m_movement_speed(SPEED), m_mouse_sensitivity(SENSITIVITY), m_zoom(ZOOM)
{
    m_position = pos;
    m_up = up;
    m_yaw = yaw;
    m_pitch = pitch;
    // m_right = MathHelper::vec3_create(1.0f, 0.0f, 0.0f);
    update_camera_vectors();
}

joj::Camera::~Camera() = default;

void joj::Camera::process_keyboard(const CameraMovement direction, const f32 dt)
{
    const f32 velocity = m_movement_speed * dt;
    if (direction == CameraMovement::FORWARD)
        m_position = MathHelper::vec3_add(m_position, MathHelper::vec3_mul_by_scalar(m_target, velocity));
    if (direction == CameraMovement::BACKWARD)
        m_position = MathHelper::vec3_minus(m_position, MathHelper::vec3_mul_by_scalar(m_target, velocity));
    if (direction == CameraMovement::LEFT)
        m_position = MathHelper::vec3_minus(m_position, MathHelper::vec3_mul_by_scalar(m_right, velocity));
    if (direction == CameraMovement::RIGHT)
        m_position = MathHelper::vec3_add(m_position, MathHelper::vec3_mul_by_scalar(m_right, velocity));
}

void joj::Camera::process_mouse_movement(f32 xoffset, f32 yoffset, const b8 constrain_pitch)
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

void joj::Camera::process_mouse_scroll(const f32 yoffset)
{
    m_zoom -= yoffset;
    if (m_zoom < 1.0f)
        m_zoom = 1.0f;
    if (m_zoom > 45.0f)
        m_zoom = 45.0f;
}

void joj::Camera::update_camera_vectors()
{
    Vector3 front = MathHelper::vec3_create(1.0f, 1.0f, 1.0f);
    front.x = static_cast<f32>(cos(MathHelper::to_radians(m_yaw)) * cos(MathHelper::to_radians(m_pitch)));
    front.y = static_cast<f32>(sin(MathHelper::to_radians(m_pitch)));
    front.z = static_cast<f32>(sin(MathHelper::to_radians(m_yaw)) * cos(MathHelper::to_radians(m_pitch)));
    m_target = MathHelper::vec3_normalized(front);

    m_right = MathHelper::vec3_normalized(MathHelper::vec3_cross(m_target, m_up));
    m_up = MathHelper::vec3_normalized(MathHelper::vec3_cross(m_right, m_target));
}
