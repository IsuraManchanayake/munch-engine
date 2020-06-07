#pragma once

#include "Graphics/Mesh.h"
#include "Graphics/Shader.h"

struct Entity {
    Entity();

    void setup();
    void update();
    
    std::vector<Mesh> meshes;
    Shader* shader;
};