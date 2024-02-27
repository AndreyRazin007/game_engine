#include "game_engine_core/window.hpp"
#include "game_engine_core/log.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace game_engine {
    static bool s_GLFWInitialized = false;

    Window::Window(std::string title, const unsigned int width, const unsigned int height)
        : m_title{std::move(title)}, m_width{width}, m_height{height} {
            int code = init();
    }

    Window::~Window() {
        shutdown();
    }

    int Window::init() {
        LOG_INFO("Creating window {0} with size {1}x{2}", m_title, m_width, m_height);

        if (!s_GLFWInitialized) {
            if (!glfwInit()) {
                LOG_CRITICAL("Can't initialize GLFW!");
                return -1;
            }

            s_GLFWInitialized = true;
        }

        m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
        if (!m_window) {
            LOG_CRITICAL("Can't create window {0} with size {1}x{2}", m_title, m_width, m_height);
            glfwTerminate();
            return -2;
        }

        glfwMakeContextCurrent(m_window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LOG_CRITICAL("Falied to initialize GLAD");

            return -3;
        }

        return 0;
    }

    void Window::shutdown() {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void Window::onUpdate() {
        glClearColor(1, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(m_window);

        glfwPollEvents();
    }
}
