#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <array>

#include "Graphics/ShadowMaps/OmniDirectionalShadowMap.h"

struct PointLight {
    PointLight();
    ~PointLight();

    void setPoint(const glm::vec3& color, const glm::vec3& position, const glm::vec3& constants, GLfloat intensity);
    void setShadowMap(GLuint width, GLuint height, GLfloat near, GLfloat far);
    void use(GLint colorLocation, GLint positionLocation, GLint constantsLocation, GLint intensityLocation);
    void updateLightProjectionView();

    glm::vec3 color;
    glm::vec3 position;
    glm::vec3 constants;
    GLfloat intensity;

    OmniDirectionalShadowMap map;
    std::array<glm::mat4, 6> lightTransforms;
};