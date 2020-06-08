#include "Window/EditorWindow.h"
#include "UI/UILayer.h"

EditorWindow::EditorWindow(GLuint width, GLuint height, GLuint posX, GLuint posY, std::string title)
    : AbstractWindow(width, height, posX, posY, title) {
}

void EditorWindow::setup() {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    addLayer<UILayer>("UI", bufWidth, bufHeight);
    // addLayer<GraphicLayer>("Graphics", bufWidth, bufHeight);
}

void EditorWindow::update() {
}