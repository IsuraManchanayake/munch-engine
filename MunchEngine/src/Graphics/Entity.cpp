#include "Graphics/Entity.h"

Entity::Entity() 
    : model(nullptr), transform(1.f), loadedFromDisk(false) {
}

Entity::~Entity() {
    if(!loadedFromDisk) {
        delete model;
    }
}

void Entity::render(Shader* shader) {
    shader->use();
    shader->setm4("model", transform);
    for(auto& mesh : model->meshes) {
        mesh.render();
    }
}
