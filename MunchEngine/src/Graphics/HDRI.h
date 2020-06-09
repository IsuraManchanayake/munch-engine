#pragma once

#include "Graphics/Mesh.h"
#include "Graphics/Shader.h";
#include "Graphics/Texture.h"

struct HDRI {
    HDRI();
    ~HDRI();

    void create(const std::string& path);
    void render(const glm::mat4& view, const glm::mat4& projection);
    void read(GLenum textureUnit);

    Mesh mesh;
    Shader shader;
    Texture texture;
    unsigned textureLocation;
};