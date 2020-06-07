#include "Graphics/ShadowMaps/OmniDirectionalShadowMap.h"
#include "Core/Common.h"
#include "Core/Logger.h"

OmniDirectionalShadowMap::OmniDirectionalShadowMap()
    : Resource(), fbo(), map(), width(), height() {
}

OmniDirectionalShadowMap::~OmniDirectionalShadowMap() {
    clear();
}

void OmniDirectionalShadowMap::clear() {
    if(copied) return;
    if(fbo) {
        glDeleteFramebuffers(1, &fbo);
    }
    if(map) {
        glDeleteTextures(1, &map);
    }
    fbo = map = 0;
}

bool OmniDirectionalShadowMap::init(GLuint width, GLuint height, GLfloat near, GLfloat far) {
    this->width = width;
    this->height = height;
    this->near = near;
    this->far = far;

    glGenFramebuffers(1, &fbo);
    glGenTextures(1, &map);
    glBindTexture(GL_TEXTURE_CUBE_MAP, map);

    for(size_t i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 
                     width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, map, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    
    if(!validate()) {
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

void OmniDirectionalShadowMap::write() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void OmniDirectionalShadowMap::read(GLenum textureUnit) {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, map);
}

bool OmniDirectionalShadowMap::validate() {
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE) {
        Logger::error("Framebuffer error ", status);
        error_exit(1);
        return false;
    }
    return true;
}