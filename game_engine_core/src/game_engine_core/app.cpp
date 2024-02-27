#include "game_engine_core/app.hpp"
#include "game_engine_core/log.hpp"
#include "game_engine_core/window.hpp"

#include <iostream>

namespace game_engine {
    App::App() {
        LOG_INFO("Starting application");
    }

    App::~App() {
        LOG_INFO("Closing application");
    }

    int App::start(unsigned int window_width, unsigned int window_height,
                   const char *title) {
        m_window = std::make_unique<Window>(title, window_width, window_height);        

        while (true) {
            m_window->onUpdate();
            onUpdate();
        }

        return 0;
    }
}
