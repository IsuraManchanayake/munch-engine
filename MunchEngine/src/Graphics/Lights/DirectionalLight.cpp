#include "Graphics/Lights/DirectionalLight.h"

DirectionalLight::DirectionalLight() 
    : color(1.f, 1.f, 1.f), ambientIntensity(1.f), 
      diffuseDirection(0.f, 0.f, 1.f), diffuseIntensity(1.f),
      shadowMap(), lightProjectionView(1.f), shadowShader(nullptr) {
}

DirectionalLight::~DirectionalLight() {
}

void DirectionalLight::setShadowMap(GLuint width, GLuint height, GLfloat near, GLfloat far) {
    shadowMap.init(width, height, near, far);
    updateLightProjectionView();
}

void DirectionalLight::updateLightProjectionView() {
    static const float size = 30.f;
    lightProjectionView = glm::ortho(-size, size, -size, size, shadowMap.near, shadowMap.far)
        * glm::lookAt(diffuseDirection * -15.f, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
}
    
void DirectionalLight::setLight(const glm::vec3& color, GLfloat ambientIntensity, const glm::vec3& diffuseDirection, GLfloat diffuseIntensity) {
    this->color = color;
    this->ambientIntensity = ambientIntensity;
    this->diffuseDirection = diffuseDirection;
    this->diffuseIntensity = diffuseIntensity;
}

void DirectionalLight::setShadowShader(Shader* shader) {
    this->shadowShader = shader;
}

void DirectionalLight::use(Shader* shader) {
    shader->use();
    shader->setf3("directionalLight.color", color);
    shader->setf1("directionalLight.ambientIntensity", ambientIntensity);
    shader->setf3("directionalLight.diffuseDirection", diffuseDirection);
    shader->setf1("directionalLight.diffuseIntensity", diffuseIntensity);
    shader->setm4("directionalLightTransform", lightProjectionView);
    shader->settx2d("directionalLight.shadowMap", shadowMap.textureId);
}