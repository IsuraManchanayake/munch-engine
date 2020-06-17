#pragma once

#include "Graphics/Entity.h"

struct TerrainEntity : Entity {
    TerrainEntity();
    
    void render() override;
    void attachShader(Shader* shader);

    Shader* shader;
    Texture heightMap;
};