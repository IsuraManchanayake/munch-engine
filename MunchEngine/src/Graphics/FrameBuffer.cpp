#include "Graphics/FrameBuffer.h"
#include "Core/Logger.h"

FrameBuffer::FrameBuffer() 
    : Resource(), width(), height(), fbo(), textureId() {
}

FrameBuffer::~FrameBuffer() {
    clear();
}

void FrameBuffer::clear() {
    if(copied) return;
    if(fbo) {
        glDeleteFramebuffers(1, &fbo);
    }
    if(textureId) {
        glDeleteTextures(1, &textureId);
    }
    copied = width = height = fbo = textureId = 0;
}

void FrameBuffer::create(unsigned width, unsigned height) {
    this->width = width;
    this->height = height;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureId, 0);
    GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        Logger::error("Framebuffer is not complete!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::write() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void FrameBuffer::detach() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

