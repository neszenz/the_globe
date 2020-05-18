#include "camera.hpp"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#define WORLD_UP glm::vec3(0.0f, 1.0f, 0.0f)
#define WORLD_RIGHT glm::vec3(1.0f, 0.0f, 0.0f)
#define WORLD_FRONT glm::vec3(0.0f, 0.0f, 1.0f)

Camera::Camera(float fov, float aspect, glm::vec3 offset) : m_fov(fov), m_aspect(aspect), m_offset(offset) {
    this->translate(m_offset);
}
Camera::~Camera() {
}

glm::mat4 Camera::get_proj_matrix() const {
    return glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
}
glm::mat4 Camera::get_view_matrix() const {
    return glm::translate(glm::mat4_cast(m_orientation), -m_position);
}

glm::vec3 Camera::front() const {
    return glm::mat3_cast(m_orientation)[2];
}
glm::vec3 Camera::right() const {
    return glm::mat3_cast(m_orientation)[0];
}
glm::vec3 Camera::up() const {
    return glm::mat3_cast(m_orientation)[1];
}

void Camera::set_aspect(float aspect) {
    m_aspect = aspect;
}

void Camera::reset() {
    m_fov = m_FOV;
    m_aspect = m_ASPECT;
    m_near = m_NEAR;
    m_far = m_FAR;
    this->reset_position();
    this->reset_orientation();
}
void Camera::reset_position() {
    m_position = m_POSITION;
    this->translate(m_offset);
}
void Camera::reset_orientation() {
    m_orientation = m_ORIENTATION;
}

void Camera::translate(glm::vec3 vector) {
    m_position += vector;
}
void Camera::translate_x(float value){
    this->translate(glm::vec3(value, 0.0f, 0.0f));
}
void Camera::translate_y(float value){
    this->translate(glm::vec3(0.0f, value, 0.0f));
}
void Camera::translate_z(float value){
    this->translate(glm::vec3(0.0f, 0.0f, value));
}

void Camera::move_forward(float value) {
    this->translate(-value * this->front());
}
void Camera::move_backward(float value) {
    this->move_forward(-value);
}
void Camera::move_left(float value) {
    this->translate(-value * this->right());
}
void Camera::move_right(float value) {
    this->move_left(-value);
}
void Camera::move_up(float value) {
    this->translate(value * this->up());
}
void Camera::move_down(float value) {
    this->move_up(-value);
}

void Camera::rotate(float angle, glm::vec3 axis) {
    glm::vec3 oaxis = glm::normalize(axis * m_orientation);
    m_orientation *= glm::angleAxis(angle, oaxis);
}
void Camera::yaw(float angle) {
    this->rotate(angle, glm::vec3(0.0f, -1.0f, 0.0f));
}
void Camera::yaw_world_up(float angle) {
    glm::vec3 up = this->up();
    if (up.y >= 0.0) {
        this->rotate(angle, -up);
    } else {
        this->rotate(angle, up);
    }
}
void Camera::pitch(float angle) {
    this->rotate(angle, glm::vec3(-1.0, 0.0, 0.0));
}
void Camera::roll(float angle) {
    this->rotate(angle, glm::vec3(0.0, 0.0, -1.0));
}

void Camera::orbit(float angle, glm::vec3 axis, glm::vec3 center) {
    glm::vec3 pc = center - m_position;
    this->translate(pc);

    glm::vec3 oaxis = glm::normalize(axis * m_orientation);
    m_orientation *= glm::angleAxis(-angle, oaxis);

    glm::vec3 cp = -pc * glm::angleAxis(-angle, oaxis);
    this->translate(cp);
}
void Camera::orbit_x(float angle, glm::vec3 center) {
    this->orbit(angle, WORLD_UP, center);
}
void Camera::orbit_y(float angle, glm::vec3 center) {
    this->orbit(angle, WORLD_RIGHT, center);
}
