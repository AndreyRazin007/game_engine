#include <iostream>
#include <memory>

#include "game_engine_core/app.hpp"

class MainApp : public game_engine::App {
private:
    int frame{0};

    virtual void onUpdate() override {
        // std::cout << "update frame: " << ++frame << '\n';
    }
};

int main() {
    auto app = std::make_unique<MainApp>();

    int code = app->start(1024, 768, "App");    

    return code;
}
