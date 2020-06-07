#include "Graphics/Lights/PointLight.h"

PointLight::PointLight() 
    : color(1.f, 1.f, 1.f), position(0.f, 0.f, 1.f), constants(3.f, 1.f, 1.f), intensity(1.f)
    , map(), lightTransforms() {
}

PointLight::~PointLight() {
}

void PointLight::setPoint(const glm::vec3& color, const glm::vec3& position, const glm::vec3& constants, GLfloat intensity) {
    this->color = color;
    this->position = position;
    this->constants = constants;
    this->intensity = intensity;
}

void PointLight::setShadowMap(GLuint width, GLuint height, GLfloat near, GLfloat far) {
    map.init(width, height, near, far);
    updateLightProjectionView();
}

void PointLight::updateLightProjectionView() {
    float aspect = static_cast<float>(map.width) / map.height;
    glm::mat4 lightProj = glm::perspective(glm::radians(90.f), aspect, map.near, map.far);
    lightTransforms[0] = lightProj * glm::lookAt(position, position + glm::vec3{ 1.0f,  0.0f,  0.0f}, glm::vec3{ 0.0f, -1.0f,  0.0f});
    lightTransforms[1] = lightProj * glm::lookAt(position, position + glm::vec3{-1.0f,  0.0f,  0.0f}, glm::vec3{ 0.0f, -1.0f,  0.0f});
    lightTransforms[2] = lightProj * glm::lookAt(position, position + glm::vec3{ 0.0f,  1.0f,  0.0f}, glm::vec3{ 0.0f,  0.0f,  1.0f});
    lightTransforms[3] = lightProj * glm::lookAt(position, position + glm::vec3{ 0.0f, -1.0f,  0.0f}, glm::vec3{ 0.0f,  0.0f, -1.0f});
    lightTransforms[4] = lightProj * glm::lookAt(position, position + glm::vec3{ 0.0f,  0.0f,  1.0f}, glm::vec3{ 0.0f, -1.0f,  0.0f});
    lightTransforms[5] = lightProj * glm::lookAt(position, position + glm::vec3{ 0.0f,  0.0f, -1.0f}, glm::vec3{ 0.0f, -1.0f,  0.0f});
}

void PointLight::use(GLint colorLocation, GLint positionLocation, GLint constantsLocation, GLint intensityLocation) {
    glUniform3f(colorLocation, color.r, color.g, color.b);
    glUniform3f(positionLocation, position.x, position.y, position.z);
    glUniform3f(constantsLocation, constants.x, constants.y, constants.z);
    glUniform1f(intensityLocation, intensity);
}