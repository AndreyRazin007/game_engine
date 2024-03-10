#pragma once

#include "game_engine_core/keys.hpp"

#include <functional>
#include <array>

namespace game_engine {
    enum class EventType {
        WindowResize = 0,
        WindowClose,

        KeyPressed,
        KeyReleased,

        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,

        EventsCount
    };

    struct BaseEvent {
        virtual ~BaseEvent() = default;
        virtual EventType getType() const = 0;
    };

    class EventDispatcher {
    public:
        template<typename EventType>
        void addEventListener(std::function<void(EventType&)> callback) {
            auto baseCallback = [function = std::move(callback)](BaseEvent &event) {
                if (event.getType() == EventType::type) {
                    function(static_cast<EventType&>(event));
                }
            };

            m_eventCallbacks[static_cast<size_t>(EventType::type)] = std::move(baseCallback);
        }

        void dispatch(BaseEvent &event) {
            auto &callback = m_eventCallbacks[static_cast<size_t>(event.getType())];

            if (callback) {
                callback(event);
            }
        }

    private:
        std::array<std::function<void(BaseEvent&)>, static_cast<size_t>(EventType::EventsCount)> m_eventCallbacks;
    };

    struct EventMouseMoved : public BaseEvent {
        double x;
        double y;

        static const EventType type = EventType::MouseMoved;

        EventMouseMoved(const double newX, const double newY)
            : x{newX}, y{newY} {

        }

        virtual EventType getType() const override {
            return type;
        }
    };

    struct EventWindowResize : public BaseEvent {
        int width;
        int height;

        static const EventType type = EventType::WindowResize;

        EventWindowResize(const int newWidth, const int newHeight)
            : width{newWidth}, height{newHeight} {
            
        }

        virtual EventType getType() const override {
            return type;
        }
    };

    struct EventWindowClose : public BaseEvent {
        static const EventType type = EventType::WindowClose;
        
        virtual EventType getType() const override {
            return type;
        }
    };

    struct EventKeyPressed : public BaseEvent {
        KeyCode m_keyCode;
        bool m_repeated;

        static const EventType type = EventType::KeyPressed;

        EventKeyPressed(const KeyCode keyCode, const bool repeated)
            : m_keyCode{keyCode}, m_repeated{repeated} {}
        
        virtual EventType getType() const override {
            return type;
        }
    };

    struct EventKeyReleased : public BaseEvent {
        KeyCode m_keyCode;

        static const EventType type = EventType::KeyReleased;

        EventKeyReleased(const KeyCode keyCode)
            : m_keyCode{keyCode} {}
        
        virtual EventType getType() const override {
            return type;
        }
    };

    struct EventMouseButtonPressed : public BaseEvent {
        MouseButton m_mouseButton;

        double m_positionX;
        double m_positionY;

        static const EventType type = EventType::MouseButtonPressed;

        EventMouseButtonPressed(const MouseButton mouseButton,
                                const double positionX, const double positionY)
            : m_mouseButton{mouseButton}, m_positionX{positionX}, m_positionY{positionY} {}

        virtual EventType getType() const override {
            return type;
        }
    };

    struct EventMouseButtonReleased : public BaseEvent {
        MouseButton m_mouseButton;

        double m_positionX;
        double m_positionY;

        static const EventType type = EventType::MouseButtonReleased;

        EventMouseButtonReleased(const MouseButton mouseButton,
                                 const double positionX, const double positionY)
            : m_mouseButton{mouseButton}, m_positionX(positionX) , m_positionY(positionY) {}

        virtual EventType getType() const override {
            return type;
        }
    };
}
