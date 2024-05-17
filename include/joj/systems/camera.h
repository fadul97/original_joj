#ifndef JOJ_CAMERA_H
#define JOJ_CAMERA_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "math/jmath.h"

namespace joj
{
    enum class CameraMovement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    constexpr f32 YAW = -90.0f;
    constexpr f32 PITCH = 0.0f;
    constexpr f32 SPEED = 10.0f;
    constexpr f32 SENSITIVITY = 0.1f;
    constexpr f32 ZOOM = 45.0f;

    class JAPI Camera
    {
    public:
        explicit Camera(
            Vector3 pos = Vector3{ 0.0f, 0.0f, 0.0f },
            Vector3 up = Vector3{ 0.0f, 1.0f, 0.0f },
            f32 yaw = YAW, f32 pitch = PITCH
        );
        ~Camera();

        [[nodiscard]] Matrix4 get_view_mat() const;

        void process_keyboard(CameraMovement direction, f32 dt);
        void process_mouse_movement(f32 xoffset, f32 yoffset, b8 constrain_pitch = true);
        void process_mouse_scroll(f32 yoffset);

        void move_to(f32 x, f32 y, f32 z);

    public:
        Vector3 m_position;
        Vector3 m_target;
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
    { return MathHelper::look_at_lh(m_position, MathHelper::vec3_add(m_target, m_position), m_up); }

    inline void Camera::move_to(const f32 x, const f32 y, const f32 z)
    { m_position.x = x; m_position.y = y; m_position.z = z;}
}

#endif // JOJ_CAMERA_H