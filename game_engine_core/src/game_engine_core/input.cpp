#include "game_engine_core/input.hpp"

namespace game_engine {
    bool Input::m_keysPressed[static_cast<size_t>(KeyCode::KEY_LAST)] = {};

    bool Input::isKeyPressed(const KeyCode keyCode) {
        return m_keysPressed[static_cast<size_t>(keyCode)];
    }

    void Input::pressKey(const KeyCode keyCode) {
        m_keysPressed[static_cast<size_t>(keyCode)] = true;
    }

    void Input::releaseKey(const KeyCode keyCode) {
        m_keysPressed[static_cast<size_t>(keyCode)] = false;
    }
}
