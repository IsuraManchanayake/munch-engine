#include "Graphics/Entities/Default/DefaultEntity.h"

DefaultEntity::DefaultEntity() 
    : Entity(), shader(nullptr) {
}

void DefaultEntity::render() {
    // transform
    shader->setm4("model", transform);
    
    for(size_t i = 0; i < model->meshes.size(); i++) {
#pragma message("TODO: set texture units")
        // Materials
        auto& material = model->materials[model->meshToMaterial[i]];
        shader->settx("material.albedo", material.albedo);
        shader->settx("material.normal", material.normal);
        shader->settx("material.displacement", material.displacement);
        shader->settx("material.specular", material.specular);
        shader->settx("material.gloss", material.gloss);

        auto& mesh = model->meshes[i];
        mesh.render();
    }

}

void DefaultEntity::attachShader(Shader* shader) {
    this->shader = shader;
}