#include "Window/MainWindow.h"
#include "Physics/Physics.h"
#include "Core/Color.h"
#include "Layer/Layer.h"
#include "Layer/Types/TestLayer.h"
#include "Layer/Types/GraphicLayer.h"

MainWindow::MainWindow(GLuint width, GLuint height, GLuint posX, GLuint posY)
    : AbstractWindow(width, height, posX, posY) {
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
