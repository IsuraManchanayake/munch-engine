#include "App/App.h"
#include "Physics/Physics.h"

App::App() 
    : windows() {
}

App::~App() {
    for(const auto& window : windows) {
        delete window;
    }
}

bool App::shouldClose() {
    for (const auto& window : windows) {
        if (window->shouldClose()) {
            return true;
        }
    }
    return false;
}

void App::pollEvents() {
    glfwPollEvents();
}

void App::setup() {
    Physics::init();
    for(auto& window : windows) {
        window->setup();
    }
}

void App::update() {
    Physics::update();
    for(auto& window : windows) {
        window->update();
    }
}