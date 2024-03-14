#pragma once

#include "game_engine_core/event.hpp"
#include "game_engine_core/camera.hpp"

#include <memory>

namespace game_engine {
    class App {
    public:
        App();
        virtual ~App();

        App(const App&) = delete;
        App(App&&) = delete;
        App &operator=(const App&) = delete;
        App &operator=(App&&) = delete;

        virtual int start(unsigned int window_width, unsigned int window_height,
                          const char *title);
        void close();

        virtual void onUpdate() {}
        virtual void onUIDraw() {}

        virtual void onMouseButtonEvent(const MouseButton buttonCode,
                                        const double positionX,
                                        const double positionY,
                                        const bool pressed) {}
        
        glm::vec2 getCurrentCursorPosition() const;

        float cameraPosition[3] = { 0.0f, 0.0f, 1.0f };
        float cameraRotation[3] = { 0.0f, 0.0f, 0.0f };
        float cameraFov = 60.0f;
        float cameraNearPlane = 0.1f;
        float cameraFarPlane = 100.0f;
        bool perspectiveCamera = true;
        Camera camera{glm::vec3{-5.0f, 0.0f, 0.0f}};

    private:
        void draw();

        std::unique_ptr<class Window> m_window;

        EventDispatcher m_eventDispatcher;
        bool m_isCloseWindow = false;
    };
}
