#include "Graphics/Lights/DirectionalLight.h"

DirectionalLight::DirectionalLight() 
    : color(1.f, 1.f, 1.f), ambientIntensity(1.f), 
      diffuseDirection(0.f, 0.f, 1.f), diffuseIntensity(1.f),
      shadowMap(), lightProjectionView(1.f), mainShader(nullptr), shadowShader(nullptr) {
}

DirectionalLight::~DirectionalLight() {
}

void DirectionalLight::setShadowMap(GLuint width, GLuint height, GLfloat near, GLfloat far) {
    shadowMap.init(width, height, near, far);
    updateLightProjectionView();
}

void DirectionalLight::updateLightProjectionView() {
    static const float size = 20.f;
    lightProjectionView = glm::ortho(-size, size, -size, size, shadowMap.near, shadowMap.far)
        * glm::lookAt(diffuseDirection * -10.f, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
}
    
void DirectionalLight::setLight(const glm::vec3& color, GLfloat ambientIntensity, const glm::vec3& diffuseDirection, GLfloat diffuseIntensity) {
    this->color = color;
    this->ambientIntensity = ambientIntensity;
    this->diffuseDirection = diffuseDirection;
    this->diffuseIntensity = diffuseIntensity;
}

void DirectionalLight::setMainShader(Shader* shader) {
    this->mainShader = shader;
}

void DirectionalLight::setShadowShader(Shader* shader) {
    this->shadowShader = shader;
}

void DirectionalLight::use() {
    mainShader->use();
    mainShader->setf3("directionalLight.color", color);
    mainShader->setf1("directionalLight.ambientIntensity", ambientIntensity);
    mainShader->setf3("directionalLight.diffuseDirection", diffuseDirection);
    mainShader->setf1("directionalLight.diffuseIntensity", diffuseIntensity);
    mainShader->setm4("directionalLightTransform", lightProjectionView);
    mainShader->settx2d("directionalLight.shadowMap", shadowMap.textureId);
}