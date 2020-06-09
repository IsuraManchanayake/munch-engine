#pragma once

#include "Layer/Layer.h"
#include "Graphics/Scene.h"
#include "Graphics/Guide.h"

struct GraphicLayer : Layer {
    GraphicLayer(std::string name, int width, int height);

    void setup() override;
    void update() override;
    bool onKeyPress(KeyPressEvent& event) override;
    bool onKeyRelease(KeyReleaseEvent& event) override;
    bool onKeyRepeat(KeyRepeatEvent& event) override;
    bool onMouseMove(MouseMoveEvent& event) override;
    bool onWindowResize(WindowResizeEvent& event) override;

    Scene scene;
    KeyContainer keys;
};