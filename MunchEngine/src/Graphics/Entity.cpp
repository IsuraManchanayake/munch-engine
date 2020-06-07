#include "Graphics/Entity.h"

Entity::Entity() 
    : meshes(), shader(nullptr) {
}

void Entity::setup() {

}

void Entity::update() {
    if(!shader) return;

    shader->use();
    for(auto& mesh : meshes) {
        mesh.render();
    }
}
