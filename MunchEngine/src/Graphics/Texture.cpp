#include "Graphics/Texture.h"
#include "Core/Common.h"
#include "Core/Logger.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Graphics/stb_image.h"

Texture::Texture() 
    : Resource(), id(), width(), height(), bitDepth() {
}

Texture::~Texture() {
    clear();
}

void Texture::create(const std::string& fileLocation) {
    unsigned char* data = stbi_load(
        fileLocation.c_str(), &width, &height, &bitDepth, 0);
    if (!data) {
        Logger::error("Failed to load texture data from ", fileLocation);
        error_exit(1);
    }
    
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
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
