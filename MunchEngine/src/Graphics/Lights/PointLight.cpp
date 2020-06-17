#include "Graphics/Lights/PointLight.h"
#include "Core/Common.h"

PointLight::PointLight() 
    : color(1.f, 1.f, 1.f), position(0.f, 0.f, 1.f), constants(3.f, 1.f, 1.f), 
      intensity(1.f), shadowShader(nullptr),
      shadowMap(), lightTransforms(), index() {
}

PointLight::~PointLight() {
}

void PointLight::setPoint(unsigned index, const glm::vec3& color, const glm::vec3& position, const glm::vec3& constants, GLfloat intensity) {
    this->color = color;
    this->position = position;
    this->constants = constants;
    this->intensity = intensity;
    this->index = index;
}

void PointLight::setShadowShader(Shader* shader) {
    shadowShader = shader;
}

void PointLight::setShadowMap(GLuint width, GLuint height, GLfloat near, GLfloat far) {
    shadowMap.init(width, height, near, far);
    updateLightProjectionView();
}

void PointLight::updateLightProjectionView() {
    float aspect = static_cast<float>(shadowMap.width) / shadowMap.height;
    glm::mat4 lightProj = glm::perspective(glm::radians(90.f), aspect, shadowMap.near, shadowMap.far);
    lightTransforms[0] = lightProj * glm::lookAt(position, position + glm::vec3{ 1.0f,  0.0f,  0.0f}, glm::vec3{ 0.0f, -1.0f,  0.0f});
    lightTransforms[1] = lightProj * glm::lookAt(position, position + glm::vec3{-1.0f,  0.0f,  0.0f}, glm::vec3{ 0.0f, -1.0f,  0.0f});
    lightTransforms[2] = lightProj * glm::lookAt(position, position + glm::vec3{ 0.0f,  1.0f,  0.0f}, glm::vec3{ 0.0f,  0.0f,  1.0f});
    lightTransforms[3] = lightProj * glm::lookAt(position, position + glm::vec3{ 0.0f, -1.0f,  0.0f}, glm::vec3{ 0.0f,  0.0f, -1.0f});
    lightTransforms[4] = lightProj * glm::lookAt(position, position + glm::vec3{ 0.0f,  0.0f,  1.0f}, glm::vec3{ 0.0f, -1.0f,  0.0f});
    lightTransforms[5] = lightProj * glm::lookAt(position, position + glm::vec3{ 0.0f,  0.0f, -1.0f}, glm::vec3{ 0.0f, -1.0f,  0.0f});
}

void PointLight::use(Shader* shader) {
    shader->use();
    shader->setf3(cat("pointLights[", index, "].color"), color);
    shader->setf3(cat("pointLights[", index, "].position"), position);
    shader->setf3(cat("pointLights[", index, "].constants"), constants);
    shader->setf1(cat("pointLights[", index, "].intensity"), intensity);
    shader->settx3d(cat("pointLights[", index, "].shadowMap"), shadowMap.textureId);
    shader->setf1(cat("pointLights[", index, "].shadowFar"), shadowMap.far);
}