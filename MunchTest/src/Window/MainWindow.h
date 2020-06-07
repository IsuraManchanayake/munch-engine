#pragma once

#include "Window/AbstractWindow.h"

struct MainWindow : AbstractWindow {
    MainWindow(GLuint width, GLuint height, GLuint posX, GLuint posY, std::string title);
    ~MainWindow();

    void setup() override;
    void update() override;
};