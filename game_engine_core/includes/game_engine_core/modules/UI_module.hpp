#pragma once

struct GLFWwindow;

namespace game_engine {
    class UIModule {
    public:
        static void onWindowCreate(GLFWwindow *window);
        static void onWindowClose();
        static void onUIDrawBegin();
        static void onUIDrawEnd();
    };
}
