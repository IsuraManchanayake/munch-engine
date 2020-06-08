#pragma once

#include "Layer/Layer.h"
#include "Graphics/Shader.h"
#include "UI/UIRoot.h"

struct UILayer : Layer {
    UILayer(std::string name, int width, int height);

    void setup() override;
    void update() override;
    // bool onMouseMove(MouseMoveEvent& event) override;
    // bool onWindowResize(WindowResizeEvent& event) override;

    struct Mouse {
        float x, y, dx, dy;
        Mouse() : x(), y(), dx(), dy() {}
    } mouse;

    Shader shader;
    UIRoot root;
};