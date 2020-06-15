#include "Graphics/HDRI.h"
#include "Graphics/stb_image.h"
#include "Graphics/ShaderFactory.h"
#include "Core/Common.h"
#include "Core/Logger.h"
#include "Core/V3.h"

#include <glm/gtc/matrix_transform.hpp>

HDRI::HDRI() 
    : mesh(), shader(nullptr), texture(), textureLocation(), irradiance() {
}

HDRI::~HDRI() {
    delete shader;
    delete irradiance.shader;
}

void HDRI::create(const std::string& path) {
    shader = ShaderFactory::hdriShader();
    texture.create(path);

    shader->use();
    textureLocation = 0;
    shader->seti1("hdri", textureLocation);

    static const unsigned indices[] = {
        0, 1, 2, 2, 1, 3,
        2, 3, 5, 5, 3, 7,
        5, 7, 4, 4, 7, 6,
        4, 6, 0, 0, 6, 1,
        4, 0, 5, 5, 0, 2,
        1, 6, 3, 3, 6, 7
    };
    static const float vertices[] = {
        -1.0f,  1.0f, -1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, -1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
        -1.0f,  1.0f,  1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
         1.0f,  1.0f,  1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
        -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
         1.0f, -1.0f,  1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f
    };
    mesh.create(vertices, indices, size(vertices), size(indices));
}

void HDRI::createHDR(const std::string& path) {
    shader = ShaderFactory::hdriShader();
    texture.createHDR(path);

    shader->use();
    textureLocation = 0;
    shader->seti1("hdri", textureLocation);

    static const unsigned indices[] = {
        0, 1, 2, 2, 1, 3,
        2, 3, 5, 5, 3, 7,
        5, 7, 4, 4, 7, 6,
        4, 6, 0, 0, 6, 1,
        4, 0, 5, 5, 0, 2,
        1, 6, 3, 3, 6, 7
    };
    static const float vertices[] = {
        -1.0f,  1.0f, -1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, -1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
        -1.0f,  1.0f,  1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
         1.0f,  1.0f,  1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
        -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
         1.0f, -1.0f,  1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f
    };
    mesh.create(vertices, indices, size(vertices), size(indices));

}

void HDRI::setIrradianceMap() {
    static const unsigned mapWidth = 32;
    static const unsigned mapHeight = 32;
    static const float delta = 0.02;

    irradiance.mesh.createPlane();
    irradiance.shader = ShaderFactory::irradianceShader();
    
    glGenFramebuffers(1, &irradiance.fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, irradiance.fbo);
    glGenTextures(1, &irradiance.textureId);
    glBindTexture(GL_TEXTURE_2D, irradiance.textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, mapWidth, mapHeight, 0, GL_RGB, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, irradiance.textureId, 0);
    GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        Logger::error("Framebuffer is not complete!");
    }

    glViewport(0, 0, mapWidth, mapHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    irradiance.shader->use();
    glm::mat4 transform(1.f);
    transform = glm::scale(transform, 2.f * V3::_111);
    transform = glm::translate(transform, -0.5f * V3::_110);
    irradiance.shader->setm4("model", transform);
    irradiance.shader->settx("hdri", texture);
    irradiance.shader->setf1("delta", delta);
    irradiance.mesh.render();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void HDRI::render(const glm::mat4& view, const glm::mat4& projection) {
    glDepthMask(GL_FALSE);

    glm::mat4 translationRemovedView = glm::mat4(glm::mat3(view));

    shader->use();
    shader->setm4("view", translationRemovedView);
    shader->setm4("projection", projection);
    shader->settx("hdri", texture);
    // shader->settx2d("hdri", irradiance.textureId);
    mesh.render();

    glDepthMask(GL_TRUE);
}

void HDRI::read(GLenum textureUnit) {
    texture.use(textureUnit);
    // irradiance.texture.use(textureUnit);
}
