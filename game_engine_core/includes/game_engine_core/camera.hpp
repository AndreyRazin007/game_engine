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
        glm::mat4 getViewMatrix() const { return m_viewMatrix; }
        glm::mat4 getProjectionMatrix() const { return m_projectionMatrix; }

    private:
        void updateViewMatrix();
        void updateProjectionMatrix();

        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::mat4 m_viewMatrix;
        glm::mat4 m_projectionMatrix;
        ProjectionMode m_projectionMode;
    };
}
