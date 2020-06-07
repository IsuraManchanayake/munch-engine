#pragma once

#include "Window/AbstractWindow.h"

struct EditorWindow : AbstractWindow {
    EditorWindow(GLuint width, GLuint height, GLuint posX, GLuint posY, std::string title);

    void setup() override;
    void update() override;
};