#pragma once

struct GLFWwindow;

namespace game_engine {
    class VertexArray;

    class RendererOpenGL {
    public:
        static bool init(GLFWwindow *window);

        static void draw(const VertexArray &vertexArray);
        static void setClearColor(const float red, const float green,
                                    const float blue, const float alpha);
        static void clear();
        static void setViewport(const unsigned int width, const unsigned int height,
                                const unsigned int left_offset = 0,
                                const unsigned int bottom_offset = 0);
        static void enableDepthTest();
        static void disableDepthTest();

        static const char *getVendorStr();
        static const char *getRendererStr();
        static const char *getVersionStr();
    };
}
