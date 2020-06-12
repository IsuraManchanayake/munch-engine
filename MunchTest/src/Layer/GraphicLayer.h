#pragma once

#include "Layer/Layer.h"
#include "Graphics/Entity.h"
#include "Graphics/Lights/DirectionalLight.h"
#include "Graphics/Camera.h"

#include <vector>

struct GraphicLayer : Layer {
    GraphicLayer(std::string name, int width, int height);
    ~GraphicLayer();

    void setup() override;
    void update() override;
    bool onKeyPress(KeyPressEvent& event) override;
    bool onKeyRelease(KeyReleaseEvent& event) override;
    bool onKeyRepeat(KeyRepeatEvent& event) override;
    bool onMouseMove(MouseMoveEvent& event) override;
    bool onWindowResize(WindowResizeEvent& event) override;

    KeyContainer keys;
    std::vector<Entity*> entities;
    std::vector<Shader*> shaders;
    Shader* defaultShader;
    DirectionalLight directionalLight;
    Camera camera;
    Shader dirDebugShader;
    glm::mat4 projection;
};