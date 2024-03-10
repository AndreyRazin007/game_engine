#include "game_engine_core/input.hpp"

namespace game_engine {
    bool Input::m_keysPressed[static_cast<size_t>(KeyCode::KEY_LAST) + 1] = {};
    bool Input::m_mouseButtonsPressed[static_cast<size_t>(MouseButton::MOUSE_BUTTON_LAST) + 1] = {};

    bool Input::isKeyPressed(const KeyCode keyCode) {
        return m_keysPressed[static_cast<size_t>(keyCode)];
    }

    void Input::pressKey(const KeyCode keyCode) {
        m_keysPressed[static_cast<size_t>(keyCode)] = true;
    }

    void Input::releaseKey(const KeyCode keyCode) {
        m_keysPressed[static_cast<size_t>(keyCode)] = false;
    }

    bool Input::isMouseButtonPressed(const MouseButton mouseButton) {
        return m_mouseButtonsPressed[static_cast<size_t>(mouseButton)];
    }

    void Input::pressMouseButton(const MouseButton mouseButton) {
        m_mouseButtonsPressed[static_cast<size_t>(mouseButton)] = true;
    }

    void Input::releaseMouseButton(const MouseButton mouseButton) {
        m_mouseButtonsPressed[static_cast<size_t>(mouseButton)] = false;
    }
}
