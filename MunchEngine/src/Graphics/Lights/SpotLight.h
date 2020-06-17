#pragma once

#include "Graphics/Lights/PointLight.h"

struct SpotLight : PointLight {
    SpotLight();
    ~SpotLight();

    void setSpot(unsigned index, const glm::vec3& color, const glm::vec3& position, const glm::vec3& direction,
                 const glm::vec3& constants, GLfloat angle, GLfloat intensity);
    
    template<typename Func>
    void useShadow(Func&& renderStep);
    void use(Shader* shader);

    glm::vec3 direction;
    GLfloat angle;
};

template<typename Func>
void SpotLight::useShadow(Func&& renderStep) {
    PointLight::useShadow(renderStep);
}