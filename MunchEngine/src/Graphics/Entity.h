#pragma once

#include "Graphics/Model.h"
#include "Graphics/Material.h"
#include "Graphics/Shader.h"
// #include "Graphics/Material.h"

struct Entity {
    Model* model;
    glm::mat4 transform;
    bool loadedFromDisk;

    Entity();
    virtual ~Entity();
    
    void render(Shader* shader);
    virtual void render() = 0;
};