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

        m_eventDispatcher.addEventListener<EventMouseMoved>(
            [](EventMouseMoved &event) {
                LOG_INFO("[MouseMoved] Mouse moved to {0}x{1}", event.x, event.y);
            });

        m_eventDispatcher.addEventListener<EventWindowResize>(
            [](EventWindowResize &event) {
                LOG_INFO("[Resized] Changed size to {0}x{1}", event.width, event.height);
            });

        m_eventDispatcher.addEventListener<EventWindowClose>(
            [&](EventWindowClose &event) {
                LOG_INFO("[WindowClose]");
                m_isCloseWindow = true;
            });

        m_window->setEventCallback(
            [&](BaseEvent &event) {
                m_eventDispatcher.dispatch(event);
            });

        while (!m_isCloseWindow) {
            m_window->onUpdate();
            onUpdate();
        }

        return 0;
    }
}
