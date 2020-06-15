#pragma once

#include "Graphics/Resource.h"

#include <GL/glew.h>
#include <string>
#include <unordered_map>

struct Texture : Resource {
    Texture();
    ~Texture();

    ADD_SPECIAL_CLASS_FUNCTIONS(Texture);

    void create(const std::string& fileLocation);
    void createHDR(const std::string& fileLocation);
    void use(GLenum textureUnit);
    void clear();

    static Texture createColorTexture(unsigned width, unsigned height, float r, float g, float b);
    static Texture createColorTexture(unsigned width, unsigned height, float r);
    static Texture createImageTexture(const std::string& path);
    static Texture createImageTextureHDR(const std::string& path);

    GLuint id;
    int width, height, bitDepth;
    static std::unordered_map<std::string, GLuint> loadedTextures;
};