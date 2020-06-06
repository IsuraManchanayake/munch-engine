#pragma once

#include "Window/AbstractWindow.h"

struct MainWindow : AbstractWindow {
     MainWindow(GLuint width, GLuint height, GLuint posX, GLuint posY);
    ~MainWindow();

    void setup();
    void update();
};