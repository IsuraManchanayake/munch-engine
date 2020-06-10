#include "Window/MainWindow.h"
#include "Layer/Layer.h"
#include "Layer/GraphicLayer.h"
#include "Layer/NormalMapTest.h"

MainWindow::MainWindow(GLuint width, GLuint height, GLuint posX, GLuint posY, std::string title)
    : AbstractWindow(width, height, posX, posY, title) {
}

MainWindow::~MainWindow() {
}

void MainWindow::setup() {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // addLayer<GraphicLayer>("Graphics", bufWidth, bufHeight);
    addLayer<NormalMapTest>("NormalMapTest", bufWidth, bufHeight);
}

void MainWindow::update() {
}
