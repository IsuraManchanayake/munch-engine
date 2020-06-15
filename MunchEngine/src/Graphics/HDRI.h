#pragma once

#include "Graphics/Mesh.h"
#include "Graphics/Shader.h";
#include "Graphics/Texture.h"

struct HDRI {
    HDRI();
    ~HDRI();

    void create(const std::string& path);
    void createHDR(const std::string& path);
    void setIrradianceMap();
    void render(const glm::mat4& view, const glm::mat4& projection);
    void read(GLenum textureUnit);

    Mesh mesh;
    Shader* shader;
    Texture texture;
    unsigned textureLocation;

    struct {
        Mesh mesh;
        Shader* shader;
        unsigned textureId;
        unsigned fbo;
    } irradiance;
};