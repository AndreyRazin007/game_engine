#include "game_engine_core/window.hpp"
#include "game_engine_core/log.hpp"
#include "game_engine_core/rendering/OpenGL/shader_program.hpp"
#include "game_engine_core/rendering/OpenGL/vertex_buffer.hpp"
#include "game_engine_core/rendering/OpenGL/vertex_array.hpp"
#include "game_engine_core/rendering/OpenGL/index_buffer.hpp"
#include "game_engine_core/camera.hpp"

#include "game_engine_core/rendering/OpenGL/renderer_OpenGL.hpp"

#include "GLFW/glfw3.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_glfw.h"

#include "glm/mat3x3.hpp"
#include "glm/trigonometric.hpp"

namespace game_engine {
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
           uniform mat4 view_projection_matrix;
           out vec3 color;
           void main() {
              color = vertex_color;
              gl_Position = view_projection_matrix * model_matrix * vec4(vertex_position, 1.0);
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

    float cameraPosition[3] = { 0.f, 0.f, 1.f };
    float cameraRotation[3] = { 0.f, 0.f, 0.f };
    bool perspectiveCamera = false;
    Camera camera;

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

        return 0;
    }

    void Window::shutdown() {
        if (ImGui::GetCurrentContext()) {
            ImGui::DestroyContext();
        }

        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void Window::onUpdate() {
        RendererOpenGL::setClearColor(m_backgroundColor[0], m_backgroundColor[1],
                                         m_backgroundColor[2], m_backgroundColor[3]);
        RendererOpenGL::clear();

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

        ImGui::SliderFloat3("camera position", cameraPosition, -10.0f, 10.0f);
        ImGui::SliderFloat3("camera rotation", cameraRotation, 0, 360.f);
        ImGui::Checkbox("Perspective camera", &perspectiveCamera);
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

        camera.setPositionRotation(glm::vec3(cameraPosition[0], cameraPosition[1], cameraPosition[2]),
                                   glm::vec3(cameraRotation[0], cameraRotation[1], cameraRotation[2]));
        camera.setProjectionMode(perspectiveCamera ?
                                 Camera::ProjectionMode::Perspective
                                 : Camera::ProjectionMode::Orthographic);
        shaderProgram->setMatrix_4("view_projection_matrix",
                                   camera.getProjectionMatrix() * camera.getViewMatrix());

        RendererOpenGL::draw(*vao);

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}
