#pragma once

#include "Graphics/Lights/PointLight.h"

struct SpotLight : PointLight {
    SpotLight();
    ~SpotLight();

    void setSpot(const glm::vec3& color, const glm::vec3& position, const glm::vec3& direction,
                 const glm::vec3& constants, GLfloat angle, GLfloat intensity);
    void use(GLint colorLocation, GLint positionLocation, GLint directionLocation, 
             GLint constantsLocation, GLint angleLocation, GLint intensityLocation);

    glm::vec3 direction;
    GLfloat angle;
};