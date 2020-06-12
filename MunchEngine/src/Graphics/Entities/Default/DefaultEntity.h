#pragma once

#include "Graphics/Entity.h"

struct DefaultEntity : Entity {
    DefaultEntity();
    
    void render() override;
    void attachShader(Shader* shader);

    Shader* shader;
};