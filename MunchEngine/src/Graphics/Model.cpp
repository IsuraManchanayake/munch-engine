#include "Graphics/Model.h"
#include "Core/Common.h"
#include "Core/Logger.h"

Object::Object() 
    : meshes(), albedos(), normals(), meshToTex() {
}

Object::~Object() {
}

void Object::load(const std::string& path) {
#if 1
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
    loadNode(scene->mRootNode, scene);
    loadMaterials(scene);
#else
    Mesh mesh;
    mesh.create(path);
    meshes.push_back(mesh);
    meshToTex.push_back(0);
    Texture texture;
    texture.create("Textures/african_head_diffuse.tga");
    textures.push_back(texture);
#endif
}

void Object::loadNode(aiNode* node, const aiScene* scene) {
    for(size_t i = 0; i < node->mNumMeshes; i++) {
        loadMesh(scene->mMeshes[node->mMeshes[i]], scene);
    }
    for(size_t i = 0; i < node->mNumChildren; i++) {
        loadNode(node->mChildren[i], scene);
    }
}

void Object::loadMesh(aiMesh* mesh, const aiScene* scene) {
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
    meshToTex.push_back(mesh->mMaterialIndex);
}

void Object::loadMaterials(const aiScene* scene) {
    albedos.resize(scene->mNumMaterials);
    for(size_t i = 0; i < scene->mNumMaterials; i++) {
        aiMaterial* material = scene->mMaterials[i];
        
        if(material->GetTextureCount(aiTextureType_DIFFUSE)) {
            aiString path;
            if(material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
                std::string realPath = cat("Textures/", path.C_Str());
                albedos[i].create(realPath.c_str());
            }
        }
        if(albedos[i].id == 0) {
            albedos[i].create("Textures/white.tga");
        }
    }
}


std::unordered_map<std::string, Object*> Model::loadedObjects;

Model::Model()
    : object(nullptr), transform(1.f), material() {
}

Model::Model(const std::string& path, glm::mat4 transform, Material material) 
    : object(nullptr), transform(std::move(transform)), material(std::move(material)) {
    auto pos = loadedObjects.find(path);
    if(pos == loadedObjects.end()) {
        Object* newObject = new Object;
        newObject->load(path);
        pos = loadedObjects.insert({path, newObject}).first;
    }
    object = pos->second;
}


Model Model::cube(glm::mat4 transform, Material material) {
    return Model::cube(transform, material, 
                            Texture::createColorTexture(4, 4, 1.f, 1.f, 1.f), 
                            Texture::createColorTexture(4, 4, 0.5f, 0.5f, 1.0f), 
                            Texture::createColorTexture(4, 4, 0.5f, 0.5f, 0.5f));
}

Model Model::sphere(glm::mat4 transform, Material material) {
    return Model::sphere(transform, material, 
                            Texture::createColorTexture(4, 4, 1.f, 1.f, 1.f), 
                            Texture::createColorTexture(4, 4, 0.5f, 0.5f, 1.0f), 
                            Texture::createColorTexture(4, 4, 0.5f, 0.5f, 0.5f));
}

Model Model::plane(glm::mat4 transform, Material material) {
    return Model::plane(transform, material, 
                            Texture::createColorTexture(4, 4, 1.f, 1.f, 1.f), 
                            Texture::createColorTexture(4, 4, 0.5f, 0.5f, 1.0f), 
                            Texture::createColorTexture(4, 4, 0.5f, 0.5f, 0.5f));
}

Model Model::terrain(glm::mat4 transform, Material material) {
    return Model::terrain(transform, material, 
                            Texture::createColorTexture(4, 4, 1.f, 1.f, 1.f), 
                            Texture::createColorTexture(4, 4, 0.5f, 0.5f, 1.0f), 
                            Texture::createColorTexture(4, 4, 0.5f, 0.5f, 0.5f));
}


Model Model::cube(glm::mat4 transform, Material material, Texture albedo, Texture normal, Texture displacement) {
    auto pos = loadedObjects.find("cube");
    if(pos == loadedObjects.end()) {
        Mesh mesh;
        mesh.createCube();
        Object* newObject = new Object;
        newObject->meshes.push_back(mesh);
        newObject->albedos.push_back(albedo);
        newObject->normals.push_back(normal);
        newObject->displacements.push_back(displacement);
        newObject->meshToTex.push_back(0);
        pos = loadedObjects.insert({cat("cube", albedo.id, ":", normal.id, ":", displacement.id), newObject}).first;
    }
    Model model;
    model.object = pos->second;
    model.transform = std::move(transform);
    model.material = std::move(material);
    return model;
}

Model Model::sphere(glm::mat4 transform, Material material, Texture albedo, Texture normal, Texture displacement) {
    auto pos = loadedObjects.find("sphere");
    if(pos == loadedObjects.end()) {
        Mesh mesh;
        mesh.createSphere();
        Object* newObject = new Object;
        newObject->meshes.push_back(mesh);
        newObject->albedos.push_back(albedo);
        newObject->normals.push_back(normal);
        newObject->displacements.push_back(displacement);
        newObject->meshToTex.push_back(0);
        pos = loadedObjects.insert({cat("sphere", albedo.id, ":", normal.id, ":", displacement.id), newObject}).first;
    }
    Model model;
    model.object = pos->second;
    model.transform = std::move(transform);
    model.material = std::move(material);
    return model;
}

Model Model::plane(glm::mat4 transform, Material material, Texture albedo, Texture normal, Texture displacement) {
    auto pos = loadedObjects.find("plane");
    if(pos == loadedObjects.end()) {
        Mesh mesh;
        mesh.createPlane();
        Object* newObject = new Object;
        newObject->meshes.push_back(mesh);
        newObject->albedos.push_back(albedo);
        newObject->normals.push_back(normal);
        newObject->displacements.push_back(displacement);
        newObject->meshToTex.push_back(0);
        pos = loadedObjects.insert({cat("plane", albedo.id, ":", normal.id, ":", displacement.id), newObject}).first;
    }
    Model model;
    model.object = pos->second;
    model.transform = std::move(transform);
    model.material = std::move(material);
    return model;
}

Model Model::terrain(glm::mat4 transform, Material material, Texture albedo, Texture normal, Texture displacement) {
    auto pos = loadedObjects.find("terrain");
    if(pos == loadedObjects.end()) {
        Mesh mesh;
        mesh.createTerrain();
        Object* newObject = new Object;
        newObject->meshes.push_back(mesh);
        newObject->albedos.push_back(albedo);
        newObject->normals.push_back(normal);
        newObject->displacements.push_back(displacement);
        newObject->meshToTex.push_back(0);
        pos = loadedObjects.insert({cat("terrain", albedo.id, ":", normal.id, ":", displacement.id), newObject}).first;
    }
    Model model;
    model.object = pos->second;
    model.transform = std::move(transform);
    model.material = std::move(material);
    return model;
}
