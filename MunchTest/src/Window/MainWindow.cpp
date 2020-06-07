#include "Window/MainWindow.h"
#include "Layer/Layer.h"
#include "Layer/GraphicLayer.h"

MainWindow::MainWindow(GLuint width, GLuint height, GLuint posX, GLuint posY, std::string title)
    : AbstractWindow(width, height, posX, posY, title) {
}

MainWindow::~MainWindow() {
}

void MainWindow::setup() {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    addLayer<Layer>("Base", bufWidth, bufHeight);
    addLayer<GraphicLayer>("Graphics", bufWidth, bufHeight);
}

void MainWindow::update() {
}
