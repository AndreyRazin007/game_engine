#include <iostream>
#include <memory>

#include "game_engine_core/input.hpp"
#include "game_engine_core/app.hpp"

#include "imgui/imgui.h"

class GameEngineEditor : public game_engine::App {
    using KeyCode_t = game_engine::KeyCode;
    using Input_t = game_engine::Input;

private:
    virtual void onUpdate() override {
        bool isMovementCamera = false;

        glm::vec3 movementDelta{ 0, 0, 0 };
        glm::vec3 rotationDelta{ 0, 0, 0 };

        if (Input_t::isKeyPressed(KeyCode_t::KEY_W)) {
            movementDelta.x += 0.005f;
            isMovementCamera = true;
        } else if (Input_t::isKeyPressed(KeyCode_t::KEY_S)) {
            movementDelta.x -= 0.005f;
            isMovementCamera = true;
        } else if (Input_t::isKeyPressed(KeyCode_t::KEY_A)) {
            movementDelta.y -= 0.005f;
            isMovementCamera = true;
        } else if (Input_t::isKeyPressed(KeyCode_t::KEY_D)) {
            movementDelta.y += 0.005f;
            isMovementCamera = true;
        } else if (Input_t::isKeyPressed(KeyCode_t::KEY_E)) {
            movementDelta.z += 0.005f;
            isMovementCamera = true;
        } else if (Input_t::isKeyPressed(KeyCode_t::KEY_Q)) {
            movementDelta.z -= 0.005f;
            isMovementCamera= true;
        } else if (Input_t::isKeyPressed(KeyCode_t::KEY_UP)) {
            rotationDelta.y -= 0.001f;
            isMovementCamera = true;
        } else if (Input_t::isKeyPressed(KeyCode_t::KEY_DOWN)) {
            rotationDelta.y += 0.001f;
            isMovementCamera = true;
        } else if (Input_t::isKeyPressed(KeyCode_t::KEY_RIGHT)) {
            rotationDelta.z -= 0.001f;
            isMovementCamera = true;
        } else if (Input_t::isKeyPressed(KeyCode_t::KEY_LEFT)) {
            rotationDelta.z += 0.001f;
            isMovementCamera = true;
        } else if (Input_t::isKeyPressed(KeyCode_t::KEY_P)) {
            rotationDelta.x += 0.001f;
            isMovementCamera = true;
        } else if (Input_t::isKeyPressed(KeyCode_t::KEY_O)) {
            rotationDelta.x -= 0.001f;
            isMovementCamera = true;
        }

        if (isMovementCamera) {
            camera.addMovementAndRotation(movementDelta, rotationDelta);
        }
    }

    virtual void onUIDraw() override {
        cameraPosition[0] = camera.getCameraPosition().x;
        cameraPosition[1] = camera.getCameraPosition().y;
        cameraPosition[2] = camera.getCameraPosition().z;
        cameraRotation[0] = camera.getCameraRotation().x;
        cameraRotation[1] = camera.getCameraRotation().y;
        cameraRotation[2] = camera.getCameraRotation().z;

        ImGui::Begin("Editor");

        if (ImGui::SliderFloat3("camera position", cameraPosition, -10.0f, 10.0f)) {
            camera.setPosition(glm::vec3{cameraPosition[0], cameraPosition[1],
                                         cameraPosition[2]});
        }

        if (ImGui::SliderFloat3("camera rotation", cameraRotation, 0, 360.0f)) {
            camera.setRotation(glm::vec3{cameraRotation[0], cameraRotation[1],
                                         cameraRotation[2]});
        }

        ImGui::Checkbox("Perspective camera", &perspectiveCamera);
        ImGui::End();
    }
};

int main() {
    auto gameEngineEditor = std::make_unique<GameEngineEditor>();

    int returnCode = gameEngineEditor->start(1800, 900, "Game Engine");

    return returnCode;
}
