#include "game_engine_core/app.hpp"
#include "game_engine_core/log.hpp"

#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace game_engine {
    App::App() {
        LOG_INFO("Welcome to spdlog!");
        LOG_ERROR("Some error message with arg: {}", 1);
        
        LOG_WARNING("Easy padding in numbers like {:08d}", 12);
        LOG_CRITICAL("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
        LOG_INFO("Support for floats {:03.2f}", 1.23456);
        LOG_INFO("Positional args are {1} {0}..", "too", "supported");
        LOG_INFO("{:<30}", "left aligned");
    }

    App::~App() {

    }

    int App::start(unsigned int window_width, unsigned int window_height,
                   const char *title) {
        GLFWwindow *window;

        if (!glfwInit()) {
            return -1;
        }

        window = glfwCreateWindow(window_width, window_height, title, NULL, NULL);
        if (!window) {
            glfwTerminate();
            return -1;
        }

        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LOG_CRITICAL("Falied to initialize GLAD");

            return -1;
        }

        glClearColor(1, 0, 0, 0);

        while (!glfwWindowShouldClose(window)) {
            glClear(GL_COLOR_BUFFER_BIT);
            glfwSwapBuffers(window);
            glfwPollEvents();

            onUpdate();
        }

        glfwTerminate();

        return 0;
    }
}
