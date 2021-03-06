#include "Graphics/Texture.h"
#include "Core/Common.h"
#include "Core/Logger.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Graphics/stb_image.h"

std::unordered_map<std::string, GLuint> Texture::loadedTextures;

Texture::Texture() 
    : Resource(), id(), width(), height(), bitDepth() {
}

Texture::~Texture() {
    clear();
}

void Texture::create(const std::string& fileLocation) {
    auto pos = loadedTextures.find(fileLocation);
    if(pos != loadedTextures.end()) {
        id = pos->second;
        copied = true;
        return;
    }
    unsigned char* data = stbi_load(
        fileLocation.c_str(), &width, &height, &bitDepth, 0);
    if (!data) {
        Logger::error("Failed to load texture data from ", fileLocation);
        error_exit(1);
    }
    
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    GLint internalFormat = GL_RGB;
    if (bitDepth == 4) {
        internalFormat = GL_RGBA;
    } else if (bitDepth == 1) {
        internalFormat = GL_RED;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, internalFormat, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    loadedTextures.emplace(fileLocation, id);
}

void Texture::createHDR(const std::string& fileLocation) {
    auto pos = loadedTextures.find(fileLocation);
    if(pos != loadedTextures.end()) {
        id = pos->second;
        copied = true;
        return;
    }
    float* data = stbi_loadf(
        fileLocation.c_str(), &width, &height, &bitDepth, 0);
    if (!data) {
        Logger::error("Failed to load texture data from ", fileLocation);
        error_exit(1);
    }
    
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    GLint internalFormat = GL_RGB16F;
    if (bitDepth == 4) {
        internalFormat = GL_RGBA16F;
    } else if (bitDepth == 1) {
        internalFormat = GL_RED;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_RGB, GL_FLOAT, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    loadedTextures.emplace(fileLocation, id);
}

Texture Texture::createColorTexture(unsigned width, unsigned height, float r, float g, float b) {
    Texture texture;

    texture.width = width;
    texture.height = height;
    texture.bitDepth = 3;
    unsigned char* data = new unsigned char[width * height * 3];

    for(size_t i = 0, s = ((size_t)width) * height; i < s; i++) {
        data[3 * i + 0] = (unsigned char) (r * 255);
        data[3 * i + 1] = (unsigned char) (g * 255);
        data[3 * i + 2] = (unsigned char) (b * 255);
    }

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);

    delete[] data;

    return Texture {texture};
}

Texture Texture::createColorTexture(unsigned width, unsigned height, float r) {
    Texture texture;

    texture.width = width;
    texture.height = height;
    texture.bitDepth = 1;
    unsigned char* data = new unsigned char[width * height * 1];

    for(size_t i = 0, s = ((size_t)width) * height; i < s; i++) {
        data[1 * i + 0] = (unsigned char) (r * 255);
    }

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);

    delete[] data;

    return Texture {texture};
}

Texture Texture::createImageTexture(const std::string& path) {
    Texture texture;
    texture.create(path);
    return Texture {texture};
}

Texture Texture::createImageTextureHDR(const std::string& path) {
    Texture texture;
    texture.createHDR(path);
    return Texture {texture};
}

void Texture::use(GLenum textureUnit) {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::clear() {
    if(copied) return;
    if(id) {
        glDeleteTextures(1, &id);
    }
    id = width = height = bitDepth = 0;
}
