#include "Graphics/Lights/SpotLight.h"
#include "Core/Common.h"

SpotLight::SpotLight() 
    : PointLight(), direction(0.f, -1.f, 0.f), angle(glm::radians(30.f)) {
}

SpotLight::~SpotLight() {
}

void SpotLight::setSpot(unsigned index, const glm::vec3& color, const glm::vec3& position, const glm::vec3& direction,
                const glm::vec3& constants, GLfloat angleRadians, GLfloat intensity) {
    PointLight::setPoint(index, color, position, constants, intensity);
    this->direction = direction;
    this->angle = angleRadians;
}

void SpotLight::use() {
    mainShader->use();
    mainShader->setf3(cat("spotLights[", index, "].color"), color);
    mainShader->setf3(cat("spotLights[", index, "].position"), position);
    mainShader->setf3(cat("spotLights[", index, "].constants"), constants);
    mainShader->setf1(cat("spotLights[", index, "].intensity"), intensity);
    mainShader->settx3d(cat("spotLights[", index, "].shadowMap"), shadowMap.textureId);
    mainShader->setf1(cat("spotLights[", index, "].shadowFar"), shadowMap.far);
    mainShader->setf3(cat("spotLights[", index, "].direction"), direction);
    mainShader->setf1(cat("spotLights[", index, "].angle"), angle);
}