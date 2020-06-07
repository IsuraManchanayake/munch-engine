#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <array>

#include "Layer/Layer.h"

struct AbstractWindow {
    AbstractWindow();
    AbstractWindow(GLuint width, GLuint height, GLuint posX, GLuint posY);
    virtual ~AbstractWindow();

    void init();
    bool shouldClose();
    void swapBuffers();
    void setFocus();
    void setCurrentContext();
    void setupLayers();
    virtual void setup() = 0;
    void setupWindow();
    virtual void update() = 0;
    void updateLayers();
    void updateWindow();
    void handleEvent(Event& event);
    template<typename LayerType, typename... Args>
    LayerType* addLayer(Args&& ...args);

    GLFWwindow* window;
    GLuint width;
    GLuint height;
    GLuint posX;
    GLuint posY;
    GLint bufWidth;
    GLint bufHeight;
    struct Mouse {
        float x, y;
        float dx, dy;
        bool firstMoved;
        Mouse();
    } mouse;
    KeyContainer keys;
    std::vector<Layer*> layers;

    static void handleMouseMove(GLFWwindow* glfwWindow, double posX, double posY);
    static void handleMouseButton(GLFWwindow* glfwWindow, int button, int action, int mods);
    static void handleKeys(GLFWwindow* glfwWindow, int key, int code, int action, int mode);
    static void handleWindowResize(GLFWwindow* glfwWindow, int width, int height);
    void createCallbacks();
};

template<typename LayerType, typename... Args>
inline LayerType* AbstractWindow::addLayer(Args&& ...args) {
    LayerType* layer = new LayerType(std::forward<Args>(args)...);
    layers.push_back(layer);
    return layer;
}
