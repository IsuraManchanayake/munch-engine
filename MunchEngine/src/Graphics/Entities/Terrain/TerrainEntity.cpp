#include "Graphics/Entities/Terrain/TerrainEntity.h"

TerrainEntity::TerrainEntity() 
    : Entity(), shader(nullptr), heightMap() {
    heightMap = Texture::createColorTexture(4, 4, 0.f);
}

void TerrainEntity::render() {
    // transform
    shader->setm4("model", transform);
    shader->setf2("uvScale", uvScale);
    // shader->settx("heightMap", heightMap);
    
    for(size_t i = 0; i < model->meshes.size(); i++) {
#pragma message("TODO: set texture units")
        // Materials
        auto& material = model->materials[model->meshToMaterial[i]];
        shader->settx("material.albedo", material.albedo);
        shader->settx("material.normal", material.normal);
        shader->settx("pmaterial.displacement", material.displacement);
        shader->settx("material.specular", material.specular);
        shader->settx("material.gloss", material.gloss);
        shader->settx("material.roughness", material.roughness);
        shader->settx("material.metallic", material.metallic);
        shader->settx("material.ao", material.ao);

        auto& mesh = model->meshes[i];
        mesh.render();
    }

}

void TerrainEntity::attachShader(Shader* shader) {
    this->shader = shader;
}
