#pragma once

#include "game_engine_core/event.hpp"

#include <string>
#include <functional>

#include "glm/ext/vector_float2.hpp"

struct GLFWwindow;

namespace game_engine {
    class Window {
    public:
        using EventCallbackFunction = std::function<void(BaseEvent&)>;

        Window(std::string title, const unsigned int width, const unsigned int height);
        ~Window();

        Window(const Window&) = delete;
        Window(Window&&) = delete;
        Window &operator=(const Window&) = delete;
        Window &operator=(Window&&) = delete;

        void onUpdate();
        unsigned int getWidth() const { return m_data.width; }
        unsigned int getHeight() const { return m_data.height; }

        glm::vec2 getCurrentCursorPosition() const;

        void setEventCallback(const EventCallbackFunction &callback) {
            m_data.eventCallbackFunction = callback;
        }

    private:
        struct WindowData {
            std::string title;
            unsigned int width;
            unsigned int height;

            EventCallbackFunction eventCallbackFunction;
        };

        int init();
        void shutdown();

        GLFWwindow *m_window = nullptr;
        WindowData m_data;
    };
}
