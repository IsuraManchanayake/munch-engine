#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Graphics/Mesh.h"
#include "Graphics/Texture.h"
#include "Graphics/Shader.h"
#include "Graphics/Material.h"

struct Object {
    Object();
    ~Object();

    void load(const std::string& path);
    void loadNode(aiNode* node, const aiScene* scene);
    void loadMesh(aiMesh* mesh, const aiScene* scene);
    void loadMaterials(const aiScene* scene);

    std::vector<Mesh> meshes;
    std::vector<Texture> albedos;
    std::vector<Texture> normals;
    std::vector<Texture> displacements;
    std::vector<unsigned> meshToTex;
};

struct Model {
    Object* object;
    glm::mat4 transform;
    Material material;

    static std::unordered_map<std::string, Object*> loadedObjects;

    Model();
    Model(const std::string& path, glm::mat4 transform, Material material);
    static Model cube(glm::mat4 transform, Material material);
    static Model sphere(glm::mat4 transform, Material material);
    static Model plane(glm::mat4 transform, Material material);
    static Model terrain(glm::mat4 transform, Material material);
    static Model cube(glm::mat4 transform, Material material, Texture albedo, Texture normal, Texture displacement);
    static Model sphere(glm::mat4 transform, Material material, Texture albedo, Texture normal, Texture displacement);
    static Model plane(glm::mat4 transform, Material material, Texture albedo, Texture normal, Texture displacement);
    static Model terrain(glm::mat4 transform, Material material, Texture albedo, Texture normal, Texture displacement);
};