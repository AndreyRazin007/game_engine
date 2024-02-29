#include "game_engine_core/window.hpp"
#include "game_engine_core/log.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_glfw.h"

namespace game_engine {
    static bool s_GLFWInitialized = false;

    GLfloat points[] = {
        0.0f,  0.5f, 0.0f,
        0.5f,  -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    const char *stringVertexShader =
        "#version 460\n"
        "layout(location = 0) in vec3 vertex_position;"
        "layout(location = 1) in vec3 vertex_color;"
        "out vec3 color;"
        "void main() {"
        "   color = vertex_color;"
        "   gl_Position = vec4(vertex_position, 1.0);"
        "}";

    const char *stringFragmentShader =
        "#version 460\n"
        "in vec3 color;"
        "out vec4 frag_color;"
        "void main() {"
        "   frag_color = vec4(color, 1.0);"
        "}";

    GLuint shaderProgram;
    GLuint vao;

    Window::Window(std::string title, const unsigned int width, const unsigned int height)
        : m_data{{std::move(title)}, width, height} {
        int code = init();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplOpenGL3_Init();
        ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    }

    Window::~Window() {
        shutdown();
    }

    int Window::init() {
        LOG_INFO("Creating window {0} with size {1}x{2}", m_data.title, m_data.width, m_data.height);

        if (!s_GLFWInitialized) {
            if (!glfwInit()) {
                LOG_CRITICAL("Can't initialize GLFW!");
                return -1;
            }

            s_GLFWInitialized = true;
        }

        m_window = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(),
                                    nullptr, nullptr);
        if (!m_window) {
            LOG_CRITICAL("Can't create window {0} with size {1}x{2}", m_data.title, m_data.width, m_data.height);
            glfwTerminate();
            return -2;
        }

        glfwMakeContextCurrent(m_window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LOG_CRITICAL("Falied to initialize GLAD");

            return -3;
        }

        glfwSetWindowUserPointer(m_window, &m_data);

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
            glViewport(0, 0, width, height);
        });

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &stringVertexShader, nullptr);
        glCompileShader(vertexShader);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &stringFragmentShader, nullptr);
        glCompileShader(fragmentShader);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        GLuint pointsVBO = 0;
        glGenBuffers(1, &pointsVBO);
        glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

        GLuint colorsVBO = 0;
        glGenBuffers(1, &colorsVBO);
        glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        return 0;
    }

    void Window::shutdown() {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void Window::onUpdate() {
        glClearColor(m_backgroundColor[0], m_backgroundColor[1],
                     m_backgroundColor[2], m_backgroundColor[3]);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize.x = static_cast<float>(getWidth());
        io.DisplaySize.y = static_cast<float>(getHeight());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::Begin("Background color window");
        ImGui::ColorEdit4("Background color", m_backgroundColor);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}
