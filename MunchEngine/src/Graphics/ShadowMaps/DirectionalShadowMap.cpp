#include "Graphics/ShadowMaps/DirectionalShadowMap.h"
#include "Core/Common.h"
#include "Core/Logger.h"

DirectionalShadowMap::DirectionalShadowMap()
    : Resource(), fbo(), textureId(), width(), height(), near(), far() {
}

DirectionalShadowMap::~DirectionalShadowMap() {
    clear();
}

void DirectionalShadowMap::clear() {
    if(copied) return;
    if(fbo) {
        glDeleteFramebuffers(1, &fbo);
    }
    if(textureId) {
        glDeleteTextures(1, &textureId);
    }
    fbo = textureId = 0;
}

bool DirectionalShadowMap::init(GLuint width, GLuint height, GLfloat near, GLfloat far) {
    this->width = width;
    this->height = height;
    this->near = near;
    this->far = far;

    glGenFramebuffers(1, &fbo);
    
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureId, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE) {
        Logger::error("Framebuffer error ", status);    
        error_exit(1);
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void DirectionalShadowMap::write() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void DirectionalShadowMap::read(GLenum textureUnit) {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureId);
}
