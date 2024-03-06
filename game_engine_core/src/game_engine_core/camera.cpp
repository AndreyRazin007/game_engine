#include "game_engine_core/camera.hpp"

#include "glm/trigonometric.hpp"

namespace game_engine {
    Camera::Camera(const glm::vec3 &position, const glm::vec3 &rotation,
                   const ProjectionMode projectionMode)
        : m_position{position}, m_rotation{rotation}, m_projectionMode{projectionMode} {
        updateViewMatrix();
        updateProjectionMatrix();
    }

    void Camera::updateViewMatrix() {
        float rotateInRadiansX = glm::radians(-m_rotation.x);
        glm::mat4 rotateMatrixX(1, 0, 0, 0,
            0, cos(rotateInRadiansX), sin(rotateInRadiansX), 0,
            0, -sin(rotateInRadiansX), cos(rotateInRadiansX), 0,
            0, 0, 0, 1);

        float rotateInRadiansY = glm::radians(-m_rotation.y);
        glm::mat4 rotateMatrixY(cos(rotateInRadiansY), 0, -sin(rotateInRadiansY), 0,
            0, 1, 0, 0,
            sin(rotateInRadiansY), 0, cos(rotateInRadiansY), 0,
            0, 0, 0, 1);

        float rotateInRadiansZ = glm::radians(-m_rotation.z);
        glm::mat4 rotateMatrix(cos(rotateInRadiansZ), sin(rotateInRadiansZ), 0, 0,
            -sin(rotateInRadiansZ), cos(rotateInRadiansZ), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);

        glm::mat4 translateMatrix(1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            -m_position[0], -m_position[1], -m_position[2], 1);

        m_viewMatrix = rotateMatrixY * rotateMatrixX * translateMatrix;
    }

    void Camera::updateProjectionMatrix() {
        if (m_projectionMode == ProjectionMode::Perspective) {
            float r = 0.1f;
            float t = 0.1f;
            float f = 10;
            float n = 0.1f;
            m_projectionMatrix = glm::mat4(n / r, 0, 0, 0,
                                            0, n / t, 0, 0,
                                            0, 0, (-f - n) / (f - n), -1,
                                            0, 0, -2 * f * n / (f - n), 0);
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
        updateViewMatrix();
    }

    void Camera::setRotation(const glm::vec3 &rotation) {
        m_rotation = rotation;
        updateViewMatrix();
    }

    void Camera::setPositionRotation(const glm::vec3 &position, const glm::vec3 &rotation) {
        m_position = position;
        m_rotation = rotation;

        updateViewMatrix();
    }

    void Camera::setProjectionMode(const ProjectionMode projectionMode) {
        m_projectionMode = projectionMode;
        updateProjectionMatrix();
    }
}
