#pragma once

#include "Layer/Layer.h"

struct GBufferLayer : Layer {
    GBufferLayer(std::string name, int width, int height);

    void setup() override;
    void update() override;
};