#include "Graphics/Entities/EntityFactory.h"

DefaultEntity* EntityFactory::sphere() {
    Mesh mesh;
    mesh.createSphere();

    Model* model = new Model;
    model->meshes.push_back(mesh);
    model->materials.push_back(Material::defaultMaterial());
    model->meshToMaterial.push_back(0);

    DefaultEntity* entity = new DefaultEntity;
    entity->model = model;
    entity->transform = glm::mat4{1.f};

    return entity;
}

DefaultEntity* EntityFactory::plane() {
    Mesh mesh;
    mesh.createPlane();

    Model* model = new Model;
    model->meshes.push_back(mesh);
    model->materials.push_back(Material::defaultMaterial());
    model->meshToMaterial.push_back(0);

    DefaultEntity* entity = new DefaultEntity;
    entity->model = model;
    entity->transform = glm::mat4{1.f};

    return entity;
}

DefaultEntity* EntityFactory::terrain() {
    Mesh mesh;
    mesh.createTerrain();

    Model* model = new Model;
    model->meshes.push_back(mesh);
    model->materials.push_back(Material::defaultMaterial());
    model->meshToMaterial.push_back(0);

    DefaultEntity* entity = new DefaultEntity;
    entity->model = model;
    entity->transform = glm::mat4{1.f};

    return entity;
}