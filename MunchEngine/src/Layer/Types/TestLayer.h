#pragma once

#include "Layer/Layer.h"

struct TestLayer : Layer {
    TestLayer(std::string name);

    bool onMouseMove(MouseMoveEvent& event) override;
    bool onMousePress(MousePressEvent& event) override;
    bool onKeyPress(KeyPressEvent& event) override;
    bool onKeyRelease(KeyReleaseEvent& event) override;
    bool onKeyRepeat(KeyRepeatEvent& event) override;
};