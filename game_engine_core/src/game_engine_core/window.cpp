#include "game_engine_core/window.hpp"
#include "game_engine_core/log.hpp"
#include "game_engine_core/modules/UI_module.hpp"

#include "game_engine_core/rendering/OpenGL/renderer_OpenGL.hpp"

#include "GLFW/glfw3.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_glfw.h"

namespace game_engine {
    Window::Window(std::string title, const unsigned int width, const unsigned int height)
        : m_data{{std::move(title)}, width, height} {
        int code = init();
    }

    Window::~Window() {
        shutdown();
    }

    int Window::init() {
        LOG_INFO("Creating window {0} with size {1}x{2}", m_data.title, m_data.width, m_data.height);

        glfwSetErrorCallback([](int errorCode, const char *description) {
            LOG_CRITICAL("GLFW error: {0}", description);
        });

        if (!glfwInit()) {
            LOG_CRITICAL("Can't initialize GLFW!");

            return -1;
        }

        m_window = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(),
                                    nullptr, nullptr);

        if (!m_window) {
            LOG_CRITICAL("Can't create window {0} with size {1}x{2}", m_data.title, m_data.width, m_data.height);

            return -2;
        }

        if (!RendererOpenGL::init(m_window)) {
            LOG_CRITICAL("Failed to initialize OpenGL renderer");

            return -3;
        }

        glfwSetWindowUserPointer(m_window, &m_data);

        glfwSetKeyCallback(m_window,
                           [](GLFWwindow *window, int key, int scancode, int action, int mods) {
            WindowData &data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            switch (action) {
                case GLFW_PRESS: {
                    EventKeyPressed event{static_cast<KeyCode>(key), false};
                    data.eventCallbackFunction(event);

                    break;
                }

                case GLFW_RELEASE: {
                    EventKeyReleased event(static_cast<KeyCode>(key));
                    data.eventCallbackFunction(event);

                    break;
                }

                case GLFW_REPEAT: {
                    EventKeyPressed event(static_cast<KeyCode>(key), true);
                    data.eventCallbackFunction(event);
                    break;
                }
            }
        });

        glfwSetMouseButtonCallback(m_window, [](GLFWwindow *window,
                                                int button, int action, int mods) {
            WindowData &data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            double positionX;
            double positionY;

            glfwGetCursorPos(window, &positionX, &positionY);

            switch (action) {
                case GLFW_PRESS: {
                    EventMouseButtonPressed event(static_cast<MouseButton>(button), positionX, positionY);
                    data.eventCallbackFunction(event);

                    break;
                }

                case GLFW_RELEASE: {
                    EventMouseButtonReleased event(static_cast<MouseButton>(button), positionX, positionY);
                    data.eventCallbackFunction(event);

                    break;
                }
            }
        });

        glfwSetWindowSizeCallback(m_window, [](GLFWwindow *window, int width, int height) {
            WindowData &data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.width = width;
            data.height = height;

            EventWindowResize event{width, height};
            data.eventCallbackFunction(event);
        });

        glfwSetCursorPosCallback(m_window, [](GLFWwindow *window, double x, double y) {
            WindowData &data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            EventMouseMoved event{x, y};
            data.eventCallbackFunction(event);
        });

        glfwSetWindowCloseCallback(m_window, [](GLFWwindow *window) {
            WindowData &data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            EventWindowClose event;
            data.eventCallbackFunction(event);
        });

        glfwSetFramebufferSizeCallback(m_window,
                                       [](GLFWwindow *window, int width, int height) {
            RendererOpenGL::setViewport(width, height);
        });

        UIModule::onWindowCreate(m_window);

        return 0;
    }

    void Window::shutdown() {
        UIModule::onWindowClose();
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void Window::onUpdate() {
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    glm::vec2 Window::getCurrentCursorPosition() const {
        double positionX;
        double positionY;

        glfwGetCursorPos(m_window, &positionX, &positionY);

        return {positionX, positionY};
    }
}
