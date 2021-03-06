#pragma once

#include "Layer/Layer.h"
#include "Graphics/Entity.h"
#include "Graphics/Camera.h"
#include "Graphics/Lights/DirectionalLight.h"
#include "Graphics/Lights/PointLight.h"
#include "Graphics/Lights/SpotLight.h"
#include "Graphics/HDRI.h"

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
    std::vector<PointLight> pointLights;
    std::vector<SpotLight> spotLights;
    Camera camera;
    HDRI hdri;
    glm::mat4 projection;
    
    // Debug variables
    Shader dirDebugShader;
};