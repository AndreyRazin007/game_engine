#pragma once

#include "game_engine_core/keys.hpp"

#include <iostream>

namespace game_engine {
    class Input {
    public:
        static bool isKeyPressed(const KeyCode keyCode);
        static void pressKey(const KeyCode keyCode);
        static void releaseKey(const KeyCode keyCode);

        static bool isMouseButtonPressed(const MouseButton mouseButton);
        static void pressMouseButton(const MouseButton mouseButton);
        static void releaseMouseButton(const MouseButton mouseButton);

    private:
        static bool m_keysPressed[];
        static bool m_mouseButtonsPressed[];
    };
}
