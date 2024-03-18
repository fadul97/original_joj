#ifndef JOJ_CAMERA_H
#define JOJ_CAMERA_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "math/vector3.h"
#include "math/matrix4.h"

namespace joj
{
    enum class CameraMovement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    const f32 YAW = -90.0f;
    const f32 PITCH = 0.0f;
    const f32 SPEED = 10.0f;
    const f32 SENSITIVITY = 0.1f;
    const f32 ZOOM = 45.0f;

    class JAPI Camera
    {
    public:
        Camera(Vector3 pos = Vector3{ 0.0f, 0.0f, 0.0f }, Vector3 up = Vector3{ 0.0f, 1.0f, 0.0f }, f32 yaw = YAW, f32 pitch = PITCH);
        ~Camera();

        Matrix4 get_view_mat() const;

        void process_keyboard(CameraMovement direction, f32 dt);
        void process_mouse_movement(f32 xoffset, f32 yoffset, b8 constrain_pitch = true);
        void process_mouse_scroll(f32 yoffset);

    private:
        Vector3 m_position;
        Vector3 m_front;
        Vector3 m_up;
        Vector3 m_right;
        Vector3 m_world_up;

        f32 m_yaw;
        f32 m_pitch;

        f32 m_movement_speed;
        f32 m_mouse_sensitivity;
        f32 m_zoom;

        void update_camera_vectors();
    };

    inline Matrix4 Camera::get_view_mat() const
    {
        DirectX::XMFLOAT3 r = DirectX::XMFLOAT3{ m_position.x + m_front.x, m_position.y + m_front.y, m_position.z + m_front.z };
        return DirectX::XMMatrixLookAtLH(
            DirectX::XMLoadFloat3(&m_position),
            DirectX::XMLoadFloat3(&r),
            DirectX::XMLoadFloat3(&m_up)
        );
    }
}

#endif // JOJ_CAMERA_H