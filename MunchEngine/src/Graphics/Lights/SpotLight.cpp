#include "Graphics/Lights/SpotLight.h"

SpotLight::SpotLight() 
    : PointLight(), direction(0.f, -1.f, 0.f), angle(30.f) {
}

SpotLight::~SpotLight() {
}

void SpotLight::setSpot(const glm::vec3& color, const glm::vec3& position, const glm::vec3& direction,
                const glm::vec3& constants, GLfloat angle, GLfloat intensity) {
    PointLight::setPoint(color, position, constants, intensity);
    this->direction = direction;
    this->angle = angle;
}

void SpotLight::use(GLint colorLocation, GLint positionLocation, GLint directionLocation, 
            GLint constantsLocation, GLint angleLocation, GLint intensityLocation) {
    PointLight::use(colorLocation, positionLocation, constantsLocation, intensityLocation);
    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
    glUniform1f(angleLocation, glm::radians(angle));
}