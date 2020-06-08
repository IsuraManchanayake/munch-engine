#include "App/App.h"
#include "Physics/Physics.h"

const unsigned App::fpsSamples = 100;

App::App() 
    : windows(), fpsCounterTime() {
}

App::~App() {
    Logger::stop();
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
    Logger::init();
    for(auto& window : windows) {
        window->setupWindow();
    }
}

void App::update() {
    Physics::update();
    fpsCounterTime += Physics::dt;
    for(auto& window : windows) {
        window->updateWindow();
    }
    if(Physics::frames % fpsSamples == 0) {
        Logger::info(((int)(100 * (fpsSamples / fpsCounterTime))) / 100.0f, " fps");
        fpsCounterTime = 0.f;
    }
}