#include "game_engine_core/window.hpp"
#include "game_engine_core/log.hpp"
#include "game_engine_core/rendering/OpenGL/shader_program.hpp"
#include "game_engine_core/rendering/OpenGL/vertex_buffer.hpp"
#include "game_engine_core/rendering/OpenGL/vertex_array.hpp"
#include "game_engine_core/rendering/OpenGL/index_buffer.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_glfw.h"

#include "glm/mat3x3.hpp"
#include "glm/trigonometric.hpp"

namespace game_engine {
    static bool s_GLFWInitialized = false;

    GLfloat positionsColorsTwo[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f
    };

    GLuint indices[] { 0, 1, 2, 3, 2, 1 };

    const char *stringVertexShader =
        R"(#version 460
           layout(location = 0) in vec3 vertex_position;
           layout(location = 1) in vec3 vertex_color;
           uniform mat4 model_matrix;
           out vec3 color;
           void main() {
              color = vertex_color;
              gl_Position = model_matrix * vec4(vertex_position, 1.0);
           }
        )";

    const char *stringFragmentShader =
        R"(#version 460
           in vec3 color;
           out vec4 fragment_color;
           void main() {
              fragment_color = vec4(color, 1.0);
           }
        )";

    std::unique_ptr<ShaderProgram> shaderProgram;
    std::unique_ptr<VertexBuffer> positionsColorsVBO;
    std::unique_ptr<VertexArray> vao;
    std::unique_ptr<IndexBuffer> indexBuffer;

    float scale[3] = { 1.0f, 1.0f, 1.0f };
    float rotate = 0.0f;
    float translate[3] = { 0.0f, 0.0f, 0.0f };

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

        shaderProgram = std::make_unique<ShaderProgram>(stringVertexShader,
                                                        stringFragmentShader);
        
        if (!shaderProgram->isCompiled()) {
            return false;
        }

        BufferLayout bufferLayout_1_vec3 {
            ShaderDataType::Float3
        };

        BufferLayout buffer_layout_2_vec3
        {
            ShaderDataType::Float3,
            ShaderDataType::Float3
        };

        vao = std::make_unique<VertexArray>();
        positionsColorsVBO = std::make_unique<VertexBuffer>(positionsColorsTwo,
            sizeof(positionsColorsTwo), buffer_layout_2_vec3);
        indexBuffer = std::make_unique<IndexBuffer>(indices,
            sizeof(indices) / sizeof(GLuint));

        vao->addVertexBuffer(*positionsColorsVBO);
        vao->setIndexBuffer(*indexBuffer);

        glm::mat3 matrix_1{4, 0, 0, 2, 8, 1, 0, 1, 0};
        glm::mat3 matrix_2{4, 2, 9, 2, 0, 4, 1, 4, 2};

        glm::mat3 result = matrix_1 * matrix_2;

        LOG_INFO("");
        LOG_INFO("|{0:3} {1:3} {2:3}|", result[0][0], result[1][0], result[2][0]);
        LOG_INFO("|{0:3} {1:3} {2:3}|", result[0][1], result[1][1], result[2][1]);
        LOG_INFO("|{0:3} {1:3} {2:3}|", result[0][2], result[1][2], result[2][2]);
        LOG_INFO("");

        glm::vec4 vector{1, 2, 3, 4};
        glm::mat4 matrixIdentity{1};

        glm::vec4 resultVector = matrixIdentity * vector;

        LOG_INFO("({0} {1} {2} {3})", resultVector.x, resultVector.y,
            resultVector.z, resultVector.w);

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

        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize.x = static_cast<float>(getWidth());
        io.DisplaySize.y = static_cast<float>(getHeight());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Background color window");
        ImGui::ColorEdit4("Background color", m_backgroundColor);
        ImGui::SliderFloat3("scale", scale, 0.0f, 2.0f);
        ImGui::SliderFloat("rotate", &rotate, 0.0f, 360.0f);
        ImGui::SliderFloat3("translate", translate, -1.0f, 1.0f);

        shaderProgram->bind();

        glm::mat4 scaleMatrix(scale[0], 0, 0, 0,
                               0, scale[1], 0, 0,
                               0, 0, scale[2], 0,
                               0, 0, 0, 1);

        float rotateInRadians = glm::radians(rotate);
        glm::mat4 rotateMatrix(cos(rotateInRadians), sin(rotateInRadians), 0, 0,
                                -sin(rotateInRadians), cos(rotateInRadians), 0, 0,
                                 0, 0, 1, 0,
                                 0, 0, 0, 1);

        glm::mat4 translateMatrix(1, 0, 0, 0,
                                   0, 1, 0, 0,
                                   0, 0, 1, 0,
                                   translate[0], translate[1], translate[2], 1);

        glm::mat4 modelMatrix = translateMatrix * rotateMatrix * scaleMatrix;
        shaderProgram->setMatrix_4("model_matrix", modelMatrix);

        vao->bind();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vao->getIndicesCount()),
            GL_UNSIGNED_INT, nullptr);

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}
