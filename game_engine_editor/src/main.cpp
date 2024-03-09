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
        if (Input_t::isKeyPressed(KeyCode_t::KEY_W)) {
            cameraPosition[2] -= 0.01f;
        } else if (Input_t::isKeyPressed(KeyCode_t::KEY_S)) {
            cameraPosition[2] += 0.01f;
        } else if (Input_t::isKeyPressed(KeyCode_t::KEY_A)) {
            cameraPosition[0] -= 0.01f;
        } else if (Input_t::isKeyPressed(KeyCode_t::KEY_D)) {
            cameraPosition[0] += 0.01f;
        } else if (Input_t::isKeyPressed(KeyCode_t::KEY_E)) {
            cameraPosition[1] += 0.01f;
        } else if (Input_t::isKeyPressed(KeyCode_t::KEY_Q)) {
            cameraPosition[1] -= 0.01f;
        } else if (Input_t::isKeyPressed(KeyCode_t::KEY_UP)) {
            cameraRotation[0] += 0.5f;
        } else if (Input_t::isKeyPressed(KeyCode_t::KEY_DOWN)) {
            cameraRotation[0] -= 0.5f;
        } else if (Input_t::isKeyPressed(KeyCode_t::KEY_RIGHT)) {
            cameraRotation[1] -= 0.5f;
        } else if (Input_t::isKeyPressed(KeyCode_t::KEY_LEFT)) {
            cameraRotation[1] += 0.5f;
        }
    }

    virtual void onUIDraw() override {
        ImGui::Begin("Editor");
        ImGui::SliderFloat3("Camera position", cameraPosition, -10.0f, 10.0f);
        ImGui::SliderFloat3("Camera rotation", cameraRotation, 0, 360.0f);
        ImGui::Checkbox("Perspective camera", &perspectiveCamera);
        ImGui::End();
    }
};

int main() {
    auto gameEngineEditor = std::make_unique<GameEngineEditor>();

    int returnCode = gameEngineEditor->start(1800, 900, "Game Engine");

    return returnCode;
}
