#pragma once

#include <GL/glew.h>

#include "Graphics/Resource.h"

struct OmniDirectionalShadowMap : Resource {
    OmniDirectionalShadowMap();
    ~OmniDirectionalShadowMap();

    void clear();

    ADD_SPECIAL_CLASS_FUNCTIONS(OmniDirectionalShadowMap)

    bool init(GLuint width, GLuint height, GLfloat near, GLfloat far);
    void write();
    void read(GLenum textureUnit);
    bool validate();

    GLuint fbo, map;
    GLuint width, height;
    GLfloat near, far;
};