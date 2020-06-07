#pragma once

#include "Graphics/Resource.h"

#include <GL/glew.h>
#include <iostream>

struct DirectionalShadowMap : Resource {
    DirectionalShadowMap();
    ~DirectionalShadowMap();

    ADD_SPECIAL_CLASS_FUNCTIONS(DirectionalShadowMap);

    bool init(GLuint width, GLuint height, GLfloat near, GLfloat far);
    void write();
    void read(GLenum textureUnit);
    void clear();

    GLuint fbo, map;
    GLuint width, height;
    GLfloat near, far;
};