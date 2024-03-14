#include "game_engine_core/camera.hpp"

#include "glm/trigonometric.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace game_engine {
    Camera::Camera(const glm::vec3 &position, const glm::vec3 &rotation,
                   const ProjectionMode projectionMode)
        : m_position{position}, m_rotation{rotation}, m_projectionMode{projectionMode} {
        updateViewMatrix();
        updateProjectionMatrix();
    }

    const glm::mat4 &Camera::getViewMatrix() {
        if (m_updateViewMatrix) {
            updateViewMatrix();
            m_updateViewMatrix = false;
        }

        return m_viewMatrix;
    }

    void Camera::updateViewMatrix() {
        const float rollInRadians = glm::radians(m_rotation.x);
        const float pitchInRadians = glm::radians(m_rotation.y);
        const float yawInRadians = glm::radians(m_rotation.z);

        const glm::mat3 rotateMatrixX(1, 0, 0,
                                      0, cos(rollInRadians), sin(rollInRadians),
                                      0, -sin(rollInRadians), cos(rollInRadians));

        const glm::mat3 rotateMatrixY(cos(pitchInRadians), 0, -sin(pitchInRadians),
                                      0, 1, 0,
                                      sin(pitchInRadians), 0, cos(pitchInRadians));

        const glm::mat3 rotateMatrixZ(cos(yawInRadians), sin(yawInRadians), 0,
                                      -sin(yawInRadians), cos(yawInRadians), 0,
                                      0, 0, 1);

        const glm::mat3 eulerRotateMatrix = rotateMatrixZ * rotateMatrixY * rotateMatrixX;
        m_direction = glm::normalize(eulerRotateMatrix * s_worldForward);
        m_right = glm::normalize(eulerRotateMatrix * s_worldRight);
        m_up = glm::cross(m_right, m_direction);

        m_viewMatrix = glm::lookAt(m_position, m_position + m_direction, m_up);
    }

    void Camera::updateProjectionMatrix() {
        if (m_projectionMode == ProjectionMode::Perspective) {
            m_projectionMatrix = glm::perspective(glm::radians(m_fieldOfView),
                m_viewportWidth / m_viewportHeight, m_nearClipPlane, m_farClipPlane);
        } else {
            float r = 2;
            float t = 2;
            float f = 100;
            float n = 0.1f;

            m_projectionMatrix = glm::mat4(1 / r, 0, 0, 0,
                                           0, 1 / t, 0, 0,
                                           0, 0, -2 / (f - n), 0,
                                           0, 0, (-f - n) / (f - n), 1);
        }
    }

    void Camera::setPosition(const glm::vec3 &position) {
        m_position = position;
        m_updateViewMatrix = true;
    }

    void Camera::setRotation(const glm::vec3 &rotation) {
        m_rotation = rotation;
        m_updateViewMatrix = true;
    }

    void Camera::setPositionRotation(const glm::vec3 &position, const glm::vec3 &rotation) {
        m_position = position;
        m_rotation = rotation;

        m_updateViewMatrix = true;
    }

    void Camera::setProjectionMode(const ProjectionMode projectionMode) {
        m_projectionMode = projectionMode;
        updateProjectionMatrix();
    }

    void Camera::setFarClipPlane(const float far)
    {
        m_farClipPlane = far;
        updateProjectionMatrix();
    }

    void Camera::setNearClipPlane(const float near)
    {
        m_nearClipPlane = near;
        updateProjectionMatrix();
    }

    void Camera::setViewportSize(const float width, const float height)
    {
        m_viewportWidth = width;
        m_viewportHeight = height;
        updateProjectionMatrix();
    }

    void Camera::setFieldOfView(const float fov)
    {
        m_fieldOfView = fov;
        updateProjectionMatrix();
    }

    void Camera::moveForward(const float delta) {
        m_position += m_direction * delta;
        m_updateViewMatrix = true;
    }

    void Camera::moveRight(const float delta) {
        m_position += m_right * delta;
        m_updateViewMatrix = true;
    }

    void Camera::moveUp(const float delta) {
        m_position += s_worldUp * delta;
        m_updateViewMatrix = true;
    }

    void Camera::addMovementAndRotation(const glm::vec3 &movementDelta,
                                        const glm::vec3 &rotationDelta) {
        m_position += m_direction * movementDelta.x;
        m_position += m_right * movementDelta.y;
        m_position += m_up * movementDelta.z;
        m_rotation += rotationDelta;

        m_updateViewMatrix = true;
    }
}
