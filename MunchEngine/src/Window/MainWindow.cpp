#include "Window/MainWindow.h"
#include "Physics/Physics.h"
#include "Core/Color.h"
#include "Layer/Layer.h"
#include "Layer/Types/TestLayer.h"

MainWindow::MainWindow(GLuint width, GLuint height, GLuint posX, GLuint posY)
    : AbstractWindow(width, height, posX, posY) {
}

MainWindow::~MainWindow() {
}

void MainWindow::setup() {
    setCurrentContext();
    addLayer<Layer>("Base");
    addLayer<TestLayer>("Test");
}

void MainWindow::update() {
    setCurrentContext();
    const float timef = Physics::currentTimeSeconds;

    auto color = Color::hsv(timef * 6.f, 50.f, 100.f);
    glClearColor(color.r, color.g, color.b, 1.f);
    // glClearColor(1.0f, 0.2f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    swapBuffers();
}
