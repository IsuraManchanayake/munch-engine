#include "Graphics/Lights/DirectionalLight.h"

DirectionalLight::DirectionalLight() 
    : ambientColor(1.f, 1.f, 1.f), ambientIntensity(1.f), 
      diffuseColor(1.f, 1.f, 1.f), diffuseDirection(0.f, 0.f, 1.f), diffuseIntensity(1.f),
      map(), lightProjectionView(1.f) {
}

DirectionalLight::~DirectionalLight() {
}

void DirectionalLight::setShadowMap(GLuint width, GLuint height, GLfloat near, GLfloat far) {
    map.init(width, height, near, far);
    updateLightProjectionView();
}

void DirectionalLight::updateLightProjectionView() {
    static const float size = 10.f;
    lightProjectionView = glm::ortho(-size, size, -size, size, map.near, map.far)
        * glm::lookAt(diffuseDirection * -5.f, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
}

void DirectionalLight::setAmbient(const glm::vec3& color, GLfloat intensity) {
    ambientColor = color;
    ambientIntensity = intensity;
}

void DirectionalLight::setDiffuse(const glm::vec3& color, const glm::vec3& direction, GLfloat intensity) {
    diffuseColor = color;
    diffuseDirection = direction;
    diffuseIntensity = intensity;
}

void DirectionalLight::use(GLint ambientColorLocation, GLint ambientIntensityLocation,
             GLint diffuseColorLocation, GLint diffuseDirectionLocation, GLint diffuseIntensityLocation) {
    glUniform3f(ambientColorLocation, ambientColor.r, ambientColor.g, ambientColor.b);
    glUniform1f(ambientIntensityLocation, ambientIntensity);
    glUniform3f(diffuseColorLocation, diffuseColor.r, diffuseColor.g, diffuseColor.b);
    glUniform3f(diffuseDirectionLocation, diffuseDirection.x, diffuseDirection.y, diffuseDirection.z);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}