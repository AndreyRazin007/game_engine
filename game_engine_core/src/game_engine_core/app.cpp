#include "game_engine_core/app.hpp"
#include "game_engine_core/log.hpp"
#include "game_engine_core/window.hpp"
#include "game_engine_core/event.hpp"
#include "game_engine_core/input.hpp"

#include "game_engine_core/rendering/OpenGL/shader_program.hpp"
#include "game_engine_core/rendering/OpenGL/vertex_buffer.hpp"
#include "game_engine_core/rendering/OpenGL/vertex_array.hpp"
#include "game_engine_core/rendering/OpenGL/index_buffer.hpp"
#include "game_engine_core/rendering/OpenGL/texture_2D.hpp"
#include "game_engine_core/camera.hpp"
#include "game_engine_core/rendering/OpenGL/renderer_OpenGL.hpp"
#include "game_engine_core/modules/UI_module.hpp"

#include "imgui/imgui.h"
#include "glm/mat3x3.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include "GLFW/glfw3.h"

#include <iostream>

namespace game_engine {
    GLfloat positionsCoords[] = {
        -1.0f, -1.f, -1.f,  1.f, 0.f,
        -1.0f,  1.f, -1.f,  0.f, 0.f,
        -1.0f, -1.f,  1.f,  1.f, 1.f,
        -1.0f,  1.f,  1.f,  0.f, 1.f,

         1.0f, -1.f, -1.f,  1.f, 0.f,
         1.0f,  1.f, -1.f,  0.f, 0.f,
         1.0f, -1.f,  1.f,  1.f, 1.f,
         1.0f,  1.f,  1.f,  0.f, 1.f
    };

    GLuint indices[] = {
        0, 1, 2, 3, 2, 1,
        4, 5, 6, 7, 6, 5,
        0, 4, 6, 0, 2, 6,
        1, 5, 3, 3, 7, 5,
        3, 7, 2, 7, 6, 2,
        1, 5, 0, 5, 0, 4
    };

    void generateCircle(unsigned char *data,
                        const unsigned int width,
                        const unsigned int height,
                        const unsigned int centerX,
                        const unsigned int centerY,
                        const unsigned int radius,
                        const unsigned char colorRed,
                        const unsigned char colorGreen,
                        const unsigned char colorBlue) {
        for (unsigned int x = 0; x < width; ++x) {
            for (unsigned int y = 0; y < height; ++y) {
                if ((x - centerX) * (x - centerY) + (y - centerY) * (y - centerY) <
                     radius * radius) {
                    data[3 * (x + width * y) + 0] = colorRed;
                    data[3 * (x + width * y) + 1] = colorGreen;
                    data[3 * (x + width * y) + 2] = colorBlue;
                }
            }
        }
    }

    void generateSmileTexture(unsigned char *data,
                              const unsigned int width,
                              const unsigned int height) {
        for (unsigned int x = 0; x < width; ++x) {
            for (unsigned int y = 0; y < height; ++y) {
                data[3 * (x + width * y) + 0] = 200;
                data[3 * (x + width * y) + 1] = 191;
                data[3 * (x + width * y) + 2] = 231;
            }
        }

        generateCircle(data, width, height, width * 0.5, height * 0.5, width * 0.4,
                       255, 255, 0);

        generateCircle(data, width, height, width * 0.5, height * 0.4, width * 0.2,
                       0, 0, 0);
        generateCircle(data, width, height, width * 0.5, height * 0.45, width * 0.2,
                       255, 255, 0);

        generateCircle(data, width, height, width * 0.35, height * 0.6, width * 0.07,
                       255, 0, 255);
        generateCircle(data, width, height, width * 0.65, height * 0.6, width * 0.07,
                       0, 0, 255);
    }

        void generateQuadsTexture(unsigned char *data,
                                    const unsigned int width,
                                    const unsigned int height) {
        for (unsigned int x = 0; x < width; ++x) {
            for (unsigned int y = 0; y < height; ++y) {
                if ((x < width / 2 && y < height / 2) || x >= width / 2 &&
                     y >= height / 2) {
                    data[3 * (x + width * y) + 0] = 0;
                    data[3 * (x + width * y) + 1] = 0;
                    data[3 * (x + width * y) + 2] = 0;
                } else {
                    data[3 * (x + width * y) + 0] = 255;
                    data[3 * (x + width * y) + 1] = 255;
                    data[3 * (x + width * y) + 2] = 255;
                }
            }
        }
    }

    const char *vertexShader =
        R"(#version 460
            layout(location = 0) in vec3 vertex_position;
            layout(location = 1) in vec2 texture_coord;

            uniform mat4 model_matrix;
            uniform mat4 view_projection_matrix;
            uniform int current_frame;

            out vec2 texture_coord_smile;
            out vec2 texture_coord_quads;

            void main() {
                texture_coord_smile = texture_coord;
                texture_coord_quads = texture_coord +
                    vec2(current_frame / 1000.0f, current_frame / 1000.0f);
                gl_Position = view_projection_matrix * model_matrix *
                    vec4(vertex_position, 1.0);
            }
        )";

    const char *fragmentShader =
        R"(#version 460
            in vec2 texture_coord_smile;
            in vec2 texture_coord_quads;

            layout (binding = 0) uniform sampler2D InTextureSmile;
            layout (binding = 1) uniform sampler2D InTextureQuads;

            out vec4 fragment_color;

            void main() {
                fragment_color = texture(InTextureSmile, texture_coord_smile) *
                    texture(InTextureQuads, texture_coord_quads);
            }
        )";

    std::unique_ptr<ShaderProgram> shaderProgram;
    std::unique_ptr<VertexBuffer> cubePositionsVBO;
    std::unique_ptr<IndexBuffer> cubeIndexBuffer;
    std::unique_ptr<Texture2D> textureSmile;
    std::unique_ptr<Texture2D> textureQuads;
    std::unique_ptr<VertexArray> vao;

    float scale[3] = { 1.0f, 1.0f, 1.0f };
    float rotate = 0.0f;
    float translate[3] = { 0.0f, 0.0f, 0.0f };
    float backgroundColor[4] = { 0.33f, 0.33f, 0.33f, 0.0f };

    std::array<glm::vec3, 5> positions = {
        glm::vec3(-2.f, -2.f, -4.f),
        glm::vec3(-5.f,  0.f,  3.f),
        glm::vec3(2.f,  1.f, -2.f),
        glm::vec3(4.f, -3.f,  3.f),
        glm::vec3(1.f, -7.f,  1.f)
    };

    App::App() {
        LOG_INFO("Starting application");
    }

    App::~App() {
        LOG_INFO("Closing application");
    }

    void App::draw() {
        RendererOpenGL::setClearColor(backgroundColor[0], backgroundColor[1],
                                      backgroundColor[2], backgroundColor[3]);
        RendererOpenGL::clear();

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
        static int currentFrame = 0;
        shaderProgram->setInt("current_frame", currentFrame++);

        shaderProgram->setMatrix_4("view_projection_matrix",
                                    camera.getProjectionMatrix() * camera.getViewMatrix());
        RendererOpenGL::draw(*vao);

        for (const glm::vec3 &currentPosition : positions) {
            glm::mat4 translateMatrix(1, 0, 0, 0,
                                      0, 1, 0, 0,
                                      0, 0, 1, 0,
                                      currentPosition[0], currentPosition[1],
                                          currentPosition[2], 1);

            shaderProgram->setMatrix_4("model_matrix", translateMatrix);
            RendererOpenGL::draw(*vao);
        }

        UIModule::onUIDrawBegin();
        onUIDraw();
        UIModule::onUIDrawEnd();

        m_window->onUpdate();
        onUpdate();
    }

    int App::start(unsigned int windowWidth, unsigned int windowHeight,
                   const char *title) {
        m_window = std::make_unique<Window>(title, windowWidth, windowHeight);
        camera.setViewportSize(static_cast<float>(windowWidth),
                               static_cast<float>(windowHeight));

        m_eventDispatcher.addEventListener<EventMouseMoved>(
            [](EventMouseMoved &event) {
                LOG_INFO("[MouseMoved] Mouse moved to {0}x{1}", event.x, event.y);
            });

        m_eventDispatcher.addEventListener<EventWindowResize>(
            [&](EventWindowResize &event) {
                LOG_INFO("[Resized] Changed size to {0}x{1}", event.width, event.height);

                camera.setViewportSize(event.width, event.height);
                draw();
            });

        m_eventDispatcher.addEventListener<EventWindowClose>(
            [&](EventWindowClose &event) {
                LOG_INFO("[WindowClose]");

                close();
            });
        
        m_eventDispatcher.addEventListener<EventMouseButtonPressed>(
            [&](EventMouseButtonPressed &event) {
                LOG_INFO("[Mouse button pressed: {0}, at ({1}, {2})",
                          static_cast<size_t>(event.m_mouseButton),
                          event.m_positionX, event.m_positionY);

                Input::pressMouseButton(event.m_mouseButton);
                onMouseButtonEvent(event.m_mouseButton,
                                   event.m_positionX, event.m_positionY,
                                   true);
            });

        m_eventDispatcher.addEventListener<EventMouseButtonReleased>(
            [&](EventMouseButtonReleased &event) {
                LOG_INFO("[Mouse button released: {0}, at ({1}, {2})",
                          static_cast<size_t>(event.m_mouseButton),
                          event.m_positionX, event.m_positionY);

                Input::releaseMouseButton(event.m_mouseButton);
                onMouseButtonEvent(event.m_mouseButton,
                                   event.m_positionX, event.m_positionY,
                                   false);
            });

        m_eventDispatcher.addEventListener<EventKeyPressed>(
            [&](EventKeyPressed &event) {
                if (event.m_keyCode <= KeyCode::KEY_Z) {
                    if (event.m_repeated) {
                        LOG_INFO("[Key pressed: {0}, repeated", static_cast<char>(event.m_keyCode));
                    } else {
                        LOG_INFO("[Key pressed: {0}", static_cast<char>(event.m_keyCode));
                    }
                }

                Input::pressKey(event.m_keyCode);
            });
        
        m_eventDispatcher.addEventListener<EventKeyReleased>(
            [&](EventKeyReleased &event) {
                if (event.m_keyCode <= KeyCode::KEY_Z) {
                    LOG_INFO("[Key released: {0}", static_cast<char>(event.m_keyCode));
                }

                Input::releaseKey(event.m_keyCode);
            });

        m_window->setEventCallback(
            [&](BaseEvent &event) {
                m_eventDispatcher.dispatch(event);
            });

        const unsigned int width = 1000;
        const unsigned int height = 1000;
        const unsigned int channels = 3;

        auto *data = new unsigned char[width * height * channels];

        generateSmileTexture(data, width, height);

        textureSmile = std::make_unique<Texture2D>(data, width, height);
        textureSmile->bind(0);

        generateQuadsTexture(data, width, height);

        textureQuads = std::make_unique<Texture2D>(data, width, height);
        textureQuads->bind(1);

        delete[] data;

        shaderProgram = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);

        if (!shaderProgram->isCompiled()) {
            return false;
        }

        BufferLayout bufferLayout_1_vec_3 {
            ShaderDataType::Float3
        };

        BufferLayout bufferLayoutVec3_Vec3_Vec2 {
            ShaderDataType::Float3,
            ShaderDataType::Float2
        };

        vao = std::make_unique<VertexArray>();
        cubePositionsVBO = std::make_unique<VertexBuffer>(positionsCoords,
            sizeof(positionsCoords), bufferLayoutVec3_Vec3_Vec2);
        cubeIndexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));

        vao->addVertexBuffer(*cubePositionsVBO);
        vao->setIndexBuffer(*cubeIndexBuffer);

        RendererOpenGL::enableDepthTest();

        while (!m_isCloseWindow) {
            draw();
        }

        m_window = nullptr;

        return 0;
    }

    glm::vec2 App::getCurrentCursorPosition() const {
        return m_window->getCurrentCursorPosition();
    }

    void App::close() {
        m_isCloseWindow = true;
    }
}
