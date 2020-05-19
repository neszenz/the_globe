#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Camera {
    public:
        Camera(float fov, float aspect, glm::vec3 offset);
        ~Camera();

        glm::mat4 get_proj_matrix() const;
        glm::mat4 get_view_matrix() const;

        glm::vec3 front() const;
        glm::vec3 right() const;
        glm::vec3 up() const;

        void set_aspect(float aspect);

        void reset();
        void reset_position();
        void reset_orientation();

        void translate(glm::vec3 vector);
        void translate_x(float value);
        void translate_y(float value);
        void translate_z(float value);

        void move_forward(float value);
        void move_backward(float value);
        void move_left(float value);
        void move_right(float value);
        void move_up(float value);
        void move_down(float value);

        void rotate(float angle, glm::vec3 axis);
        void yaw(float angle);
        void yaw_world_up(float angle);
        void pitch(float angle);
        void roll(float angle);

        void orbit(float angle, glm::vec3 axis, glm::vec3 center);
        void orbit_x(float angle, glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f));
        void orbit_y(float angle, glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f));
        void orbit_z(float angle, glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f));
    private:
        const float m_FOV = 45.0f;
        const float m_ASPECT = 1.0f;
        const float m_NEAR = 0.1f;
        const float m_FAR = 1000.0f;
        const glm::vec3 m_POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
        const glm::quat m_ORIENTATION = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));

        float m_fov = m_FOV;
        float m_aspect = m_ASPECT;
        float m_near = m_NEAR;
        float m_far = m_FAR;
        glm::vec3 m_offset = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 m_position = m_POSITION;
        glm::quat m_orientation = m_ORIENTATION;
};

#endif//CAMERA_HPP
