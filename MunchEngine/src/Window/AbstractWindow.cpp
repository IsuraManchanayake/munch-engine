#include "AbstractWindow.h"

#include "Core/Common.h"
#include "Core/Logger.h"

AbstractWindow::AbstractWindow() 
    : window(nullptr), width(800), height(600),
      posX(100), posY(100), bufWidth(), bufHeight(), mouse(), keys() {
}

AbstractWindow::AbstractWindow(GLuint width, GLuint height, GLuint posX, GLuint posY, std::string title) 
    : window(nullptr), width(width), height(height),
      posX(posX), posY(posY),bufWidth(), bufHeight(), mouse(), keys(), title(std::move(title)) {
}

AbstractWindow::~AbstractWindow() {
    glfwDestroyWindow(window);
    for(const auto& layer : layers) {
        delete layer;
    }
}

void AbstractWindow::init() {
    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    glfwSetWindowPos(window, posX, posY);
    if(!window) {
        Logger::error("GLFW Window creation failed");
        glfwTerminate();
        error_exit(1);
    }
    glfwGetFramebufferSize(window, &bufWidth, &bufHeight);
    setCurrentContext();
    createCallbacks();
    glfwSetWindowUserPointer(window, this);
    glEnable(GL_DEPTH_TEST);
}

bool AbstractWindow::shouldClose() {
    return glfwWindowShouldClose(window);
}

void AbstractWindow::swapBuffers() {
    glfwSwapBuffers(window);
}

void AbstractWindow::setFocus() {
    glfwFocusWindow(window);
}

void AbstractWindow::setCurrentContext() {
    glfwMakeContextCurrent(window);
}

void AbstractWindow::setupLayers() {
    for(auto& layer : layers) {
        layer->setup();
    }
}

void AbstractWindow::updateLayers() {
    for(auto& layer : layers) {
        layer->update();
    }
}

void AbstractWindow::setupWindow() {
    setCurrentContext();
    setup();
    setupLayers();
}

void AbstractWindow::updateWindow() {
    setCurrentContext();
    update();
    updateLayers();
    swapBuffers();
}

void AbstractWindow::handleEvent(Event& event) {
    Logger::trace("Event in window level ", event);
    for(auto it = layers.rbegin(); !event.handled && it != layers.rend(); it++) {
        (*it)->onEvent(event);
    }
}

void AbstractWindow::handleMouseMove(GLFWwindow* glfwWindow, double posX, double posY) {
    AbstractWindow* window = static_cast<AbstractWindow*>(glfwGetWindowUserPointer(glfwWindow));
    if (window->mouse.firstMoved) {
        window->mouse.x = posX;
        window->mouse.y = posY;
        window->mouse.firstMoved = false;
    }
    window->mouse.dx = posX - window->mouse.x;
    window->mouse.dy = posY - window->mouse.y;
    window->mouse.x = posX;
    window->mouse.y = posY;
    MouseMoveEvent event(window->mouse.x, window->mouse.y, window->mouse.dx, window->mouse.dy);
    window->handleEvent(event);
}

void AbstractWindow::handleMouseButton(GLFWwindow* glfwWindow, int button, int action, int mods) {
    AbstractWindow* window = static_cast<AbstractWindow*>(glfwGetWindowUserPointer(glfwWindow));
    MousePressEvent event(button, action, mods);
    window->handleEvent(event);
}

void AbstractWindow::handleKeys(GLFWwindow* glfwWindow, int key, int code, int action, int mode) {
    AbstractWindow* window = static_cast<AbstractWindow*>(glfwGetWindowUserPointer(glfwWindow));
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(glfwWindow, GL_TRUE);
    }
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            window->keys[key] = 1;
            KeyPressEvent event(window->keys, key);
            window->handleEvent(event);
        } else if(action == GLFW_REPEAT) {
            window->keys[key]++;
            KeyRepeatEvent event(window->keys, key);
            window->handleEvent(event);
        } else if (action == GLFW_RELEASE) {
            window->keys[key] = 0;
            KeyReleaseEvent event(window->keys, key);
            window->handleEvent(event);
        }
    }
}

void AbstractWindow::handleWindowResize(GLFWwindow* glfwWindow, int width, int height) {
    AbstractWindow* window = static_cast<AbstractWindow*>(glfwGetWindowUserPointer(glfwWindow));
    WindowResizeEvent event(width, height); 
    window->handleEvent(event);
}

void AbstractWindow::createCallbacks() {
    glfwSetCursorPosCallback(window, AbstractWindow::handleMouseMove);
    glfwSetMouseButtonCallback(window, AbstractWindow::handleMouseButton);
    glfwSetKeyCallback(window, AbstractWindow::handleKeys);
    glfwSetWindowSizeCallback(window, AbstractWindow::handleWindowResize);
}

AbstractWindow::Mouse::Mouse() 
    : x(), y(), dx(), dy(), firstMoved(true) {
}