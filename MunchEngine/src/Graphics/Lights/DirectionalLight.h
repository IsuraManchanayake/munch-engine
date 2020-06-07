#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Graphics/ShadowMaps/DirectionalShadowMap.h"
#include "Graphics/Shader.h"

struct DirectionalLight {
    DirectionalLight();
    ~DirectionalLight();

    void setAmbient(const glm::vec3& color, GLfloat intensity);
    void setDiffuse(const glm::vec3& color, const glm::vec3& direction, GLfloat intensity);
    void setShadowMap(GLuint width, GLuint height, GLfloat near, GLfloat far);
    void updateLightProjectionView();

    void use(GLint ambientColorLocation, GLint ambientIntensityLocation,
             GLint diffuseColorLocation, GLint diffuseDirectionLocation, GLint diffuseIntensityLocation);

    glm::vec3 ambientColor;
    GLfloat ambientIntensity;
    glm::vec3 diffuseColor;
    glm::vec3 diffuseDirection;
    GLfloat diffuseIntensity;

    DirectionalShadowMap map;
    glm::mat4 lightProjectionView;
};