#pragma once

#include <vector>

#include "Window/AbstractWindow.h"
#include "Core/Logger.h"

struct App {
    App();
    ~App();

    template<typename WindowType, typename ...Args>
    WindowType* init(Args&&... args);
    template<typename WindowType, typename ...Args>
    WindowType* addWindow(Args&&... args);
    bool shouldClose();
    void pollEvents();
    void setup();
    void update();
    
    std::vector<AbstractWindow*> windows;
    float fpsCounterTime;
    static const unsigned fpsSamples;
};

// ============== Template implementations

template<typename WindowType, typename ...Args>
inline WindowType* App::init(Args&&... args) {
    if (!glfwInit()) {
        Logger::error("GLFW initialization failed");
        glfwTerminate();
        error_exit(1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    WindowType* mainWindow = addWindow<WindowType>(std::forward<Args>(args)...);

    GLenum initStatus = glewInit();
    if (initStatus != GLEW_OK) {
        Logger::error("GLEW initialization failed");
        Logger::error(glewGetErrorString(initStatus));
        error_exit(1);
    }
    return mainWindow;
}

template<typename WindowType, typename ...Args>
inline WindowType* App::addWindow(Args&&... args) {
    WindowType* window = new WindowType(std::forward<Args>(args)...);
    window->init();
    windows.push_back(window);
    windows[0]->setFocus();
    return window;
}