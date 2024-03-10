#pragma once

#include "glm/vec3.hpp"
#include "glm/ext/matrix_float4x4.hpp"

namespace game_engine {
    class Camera {
    public:
        enum class ProjectionMode {
            Perspective,
            Orthographic
        };

        Camera(const glm::vec3 &position = { 0, 0, 0 },
               const glm::vec3 &rotation = { 0, 0, 0 },
               const ProjectionMode projectionMode = ProjectionMode::Perspective);
        
        void setPosition(const glm::vec3 &position);
        void setRotation(const glm::vec3 &rotation);
        void setPositionRotation(const glm::vec3 &position, const glm::vec3 &rotation);
        void setProjectionMode(const ProjectionMode projectionMode);

        const glm::mat4 &getViewMatrix() const { return m_viewMatrix; }
        const glm::mat4 &getProjectionMatrix() const { return m_projectionMatrix; }

        void moveForward(const float delta);
        void moveRight(const float delta);
        void moveUp(const float delta);

        const glm::vec3 &getCameraPosition() const { return m_position; }
        const glm::vec3 &getCameraRotation() const { return m_rotation; }

        void addMovementAndRotation(const glm::vec3 &movementDelta,
                                    const glm::vec3 &rotationDelta);

    private:
        void updateViewMatrix();
        void updateProjectionMatrix();

        glm::vec3 m_position;
        glm::vec3 m_rotation;

        glm::mat4 m_viewMatrix;
        glm::mat4 m_projectionMatrix;

        ProjectionMode m_projectionMode;

        glm::vec3 m_direction;
        glm::vec3 m_right;
        glm::vec3 m_up;

        static constexpr glm::vec3 s_worldUp{ 0.0f, 0.0f, 1.0f };
        static constexpr glm::vec3 s_worldRight{ 0.0f, -1.0f, 0.0f };
        static constexpr glm::vec3 s_worldForward{ 1.0f, 0.0f, 0.0f };
    };
}
