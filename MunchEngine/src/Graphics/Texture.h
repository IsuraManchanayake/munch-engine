#pragma once

#include "Graphics/Resource.h"

#include <GL/glew.h>
#include <string>

struct Texture : Resource {
    Texture();
    ~Texture();

    ADD_SPECIAL_CLASS_FUNCTIONS(Texture);

    void create(const std::string& fileLocation);
    void use(GLenum textureUnit);
    void clear();

    GLuint id;
    int width, height, bitDepth;
};