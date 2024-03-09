#pragma once

#include "game_engine_core/keys.hpp"

#include <iostream>

namespace game_engine {
    class Input {
    public:
        static bool isKeyPressed(const KeyCode keyCode);
        static void pressKey(const KeyCode keyCode);
        static void releaseKey(const KeyCode keyCode);
    
    private:
        static bool m_keysPressed[static_cast<size_t>(KeyCode::KEY_LAST)];
    };
}
