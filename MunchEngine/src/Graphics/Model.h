#pragma once

#include "Graphics/Material.h"
#include "Graphics/Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <unordered_map>

struct Model {
    Model();

    static Model* load(const std::string& path);
    void loadNode(aiNode* node, const aiScene* scene);
    void loadMesh(aiMesh* mesh, const aiScene* scene);
    void loadMaterials(const aiScene* scene);

    std::vector<Mesh> meshes;
    std::vector<Material> materials;
    std::vector<unsigned> meshToMaterial;

    static std::unordered_map<std::string, Model*> loadedModels; // only fron files
};