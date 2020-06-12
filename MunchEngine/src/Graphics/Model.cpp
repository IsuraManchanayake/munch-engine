#include "Graphics/Model.h"
#include "Core/Logger.h"

std::unordered_map<std::string, Model*> Model::loadedModels;

Model::Model() 
    : meshes(), materials(), meshToMaterial() {
}

Model* Model::load(const std::string& path) {
    auto pos = loadedModels.find(path);
    if(pos != loadedModels.end()) {
        return pos->second;
    }
    Model* model = new Model;
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate
                                                    | aiProcess_FlipUVs
                                                    | aiProcess_GenSmoothNormals
                                                    | aiProcess_JoinIdenticalVertices
                                                    );
    if(!scene) {
        Logger::error("Error loading ", path);
        error_exit(1);
    }
    model->loadNode(scene->mRootNode, scene);
    model->loadMaterials(scene);
    loadedModels.emplace(path, model);
    return model;
}

void Model::loadNode(aiNode* node, const aiScene* scene) {
    for(size_t i = 0; i < node->mNumMeshes; i++) {
        loadMesh(scene->mMeshes[node->mMeshes[i]], scene);
    }
    for(size_t i = 0; i < node->mNumChildren; i++) {
        loadNode(node->mChildren[i], scene);
    }
}

void Model::loadMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<GLfloat> vertices;
    std::vector<unsigned> indices;
    
    for(size_t i = 0; i < mesh->mNumVertices; i++) {
        vertices.insert(vertices.end(), { mesh->mVertices[i].x, 
                                          mesh->mVertices[i].y, 
                                          mesh->mVertices[i].z });
        if(mesh->mTextureCoords[0]) {
            vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, 
                                              mesh->mTextureCoords[0][i].y});
        } else {
            vertices.insert(vertices.end(), { 0.f, 
                                              1.f });
        }
        vertices.insert(vertices.end(), { mesh->mNormals[i].x, 
                                          mesh->mNormals[i].y, 
                                          mesh->mNormals[i].z });
    }

    for(size_t i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(size_t j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    Mesh newMesh;
    newMesh.create(&vertices[0], &indices[0], vertices.size(), indices.size());
    meshes.push_back(newMesh);
    meshToMaterial.push_back(mesh->mMaterialIndex);
}

void Model::loadMaterials(const aiScene* scene) {
    materials.resize(scene->mNumMaterials);
    for(size_t i = 0; i < scene->mNumMaterials; i++) {
        aiMaterial* material = scene->mMaterials[i];
        
        if(material->GetTextureCount(aiTextureType_DIFFUSE)) {
            aiString path;
            if(material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
                std::string realPath = cat("materials/", path.C_Str());
                materials[i].albedo.create(realPath.c_str());
                materials[i].attachDefault(TextureMapType::normal);
                materials[i].attachDefault(TextureMapType::displacement);
            }
        }
        if(materials[i].albedo.id == 0) {
            materials[i] = Material::defaultMaterial();
        }
    }
}