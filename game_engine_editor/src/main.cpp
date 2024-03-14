#include <iostream>
#include <memory>

#include "game_engine_core/input.hpp"
#include "game_engine_core/app.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

class GameEngineEditor : public game_engine::App {
    using KeyCode_t = game_engine::KeyCode;
    using Input_t = game_engine::Input;

private:
    double m_initialMousePositionX = 0.0;
    double m_initialMousePositionY = 0.0;

    virtual void onUpdate() override {
        glm::vec3 movementDelta{ 0, 0, 0 };
        glm::vec3 rotationDelta{ 0, 0, 0 };

        if (Input_t::isKeyPressed(KeyCode_t::KEY_W)) {
            movementDelta.x += 0.005f;
        } else if (Input_t::isKeyPressed(KeyCode_t::KEY_S)) {
            movementDelta.x -= 0.005f;
        } else if (Input_t::isKeyPressed(KeyCode_t::KEY_A)) {
            movementDelta.y -= 0.005f;
        } else if (Input_t::isKeyPressed(KeyCode_t::KEY_D)) {
            movementDelta.y += 0.005f;
        } else if (Input_t::isKeyPressed(KeyCode_t::KEY_E)) {
            movementDelta.z += 0.005f;
        } else if (Input_t::isKeyPressed(KeyCode_t::KEY_Q)) {
            movementDelta.z -= 0.005f;
        } else if (Input_t::isKeyPressed(KeyCode_t::KEY_UP)) {
            rotationDelta.y -= 0.001f;
        } else if (Input_t::isKeyPressed(KeyCode_t::KEY_DOWN)) {
            rotationDelta.y += 0.001f;
        } else if (Input_t::isKeyPressed(KeyCode_t::KEY_RIGHT)) {
            rotationDelta.z -= 0.001f;
        } else if (Input_t::isKeyPressed(KeyCode_t::KEY_LEFT)) {
            rotationDelta.z += 0.001f;
        } else if (Input_t::isKeyPressed(KeyCode_t::KEY_P)) {
            rotationDelta.x += 0.001f;
        } else if (Input_t::isKeyPressed(KeyCode_t::KEY_O)) {
            rotationDelta.x -= 0.001f;
        }

        if (game_engine::Input::isMouseButtonPressed(game_engine::MouseButton::MOUSE_BUTTON_RIGHT)) {
            camera.addMovementAndRotation(movementDelta, rotationDelta);
            glm::vec2 currentCursorPosition = getCurrentCursorPosition();

            if (game_engine::Input::isMouseButtonPressed(game_engine::MouseButton::MOUSE_BUTTON_LEFT)) {
                camera.moveRight(static_cast<float>(currentCursorPosition.x - m_initialMousePositionX) / 100.0f);
                camera.moveUp(static_cast<float>(m_initialMousePositionY - currentCursorPosition.y) / 100.0f);
            } else {
                rotationDelta.z += static_cast<float>(m_initialMousePositionX - currentCursorPosition.x) / 5.0f;
                rotationDelta.y -= static_cast<float>(m_initialMousePositionY - currentCursorPosition.y) / 5.0f;
            }

            m_initialMousePositionX = currentCursorPosition.x;
            m_initialMousePositionY = currentCursorPosition.y;
        }

        camera.addMovementAndRotation(movementDelta, rotationDelta);
    }

    void setupDockspaceMenu() {
        static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoWindowMenuButton;
        static ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        windowFlags |= ImGuiWindowFlags_NoBackground;

        const ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace", nullptr, windowFlags);
        ImGui::PopStyleVar(3);

        ImGuiIO &io = ImGui::GetIO();
        ImGuiID dockspaceId = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New Scene...", NULL)) {}
                if (ImGui::MenuItem("Open Scene...", NULL)) {}
                if (ImGui::MenuItem("Save Scene...", NULL)) {}

                ImGui::Separator();

                if (ImGui::MenuItem("Exit", NULL)) {
                    close();
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::End();
    }

    virtual void onMouseButtonEvent(const game_engine::MouseButton buttonCode,
                                    const double positionX,
                                    const double positionY,
                                    const bool pressed) override {
        m_initialMousePositionX = positionX;
        m_initialMousePositionY = positionY;
    }

    virtual void onUIDraw() override {
        setupDockspaceMenu();
        cameraPosition[0] = camera.getPosition().x;
        cameraPosition[1] = camera.getPosition().y;
        cameraPosition[2] = camera.getPosition().z;
        cameraRotation[0] = camera.getRotation().x;
        cameraRotation[1] = camera.getRotation().y;
        cameraRotation[2] = camera.getRotation().z;

        cameraFov = camera.getFieldOfView();
        cameraNearPlane = camera.getNearClipPlane();
        cameraFarPlane = camera.getFarClipPlane();

        ImGui::Begin("Editor");

        if (ImGui::SliderFloat3("camera position", cameraPosition, -10.0f, 10.0f)) {
            camera.setPosition(glm::vec3{cameraPosition[0], cameraPosition[1],
                                         cameraPosition[2]});
        }

        if (ImGui::SliderFloat3("camera rotation", cameraRotation, 0, 360.0f)) {
            camera.setRotation(glm::vec3{cameraRotation[0], cameraRotation[1],
                                         cameraRotation[2]});
        }

        if (ImGui::SliderFloat("camera FOV", &cameraFov, 1.0f, 120.0f)) {
            camera.setFieldOfView(cameraFov);
        }
        if (ImGui::SliderFloat("camera near clip plane", &cameraNearPlane, 0.1f, 10.0f))
        {
            camera.setNearClipPlane(cameraNearPlane);
        }
        if (ImGui::SliderFloat("camera far clip plane", &cameraFarPlane, 1.0f, 100.0f))
        {
            camera.setFarClipPlane(cameraFarPlane);
        }
        if (ImGui::Checkbox("Perspective camera", &perspectiveCamera))
        {
            camera.setProjectionMode(perspectiveCamera ?
                                     game_engine::Camera::ProjectionMode::Perspective :
                                     game_engine::Camera::ProjectionMode::Orthographic);
        }

        ImGui::End();
    }
};

int main() {
    auto gameEngineEditor = std::make_unique<GameEngineEditor>();

    int returnCode = gameEngineEditor->start(1800, 1000, "GameEngine Editor");

    return returnCode;
}
