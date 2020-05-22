#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Camera {
    public:
        Camera(float fov, float aspect, glm::vec3 offset = glm::vec3(0.0f));
        ~Camera();

        glm::mat4 get_proj_matrix() const;
        glm::mat4 get_view_matrix() const;

        glm::vec3 front() const;
        glm::vec3 right() const;
        glm::vec3 up() const;

        float fov() const;
        void fov(float new_fov);

        void set_aspect(float aspect);

        void reset();
        void reset_fov();
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
        // compile-time constants
        const float m_NEAR = 0.1f;
        const float m_FAR = 1000.0f;
        const glm::vec3 m_POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
        const glm::quat m_ORIENTATION = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));

        // instantation-time constants
        float m_fov_default;
        float m_aspect_default;
        glm::vec3 m_offset;

        // current state
        float m_fov;
        float m_aspect;
        float m_near;
        float m_far;
        glm::vec3 m_position;
        glm::quat m_orientation;
};

#endif//CAMERA_HPP
