#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Graphics/ShadowMaps/DirectionalShadowMap.h"
#include "Graphics/Shader.h"

struct DirectionalLight {
    DirectionalLight();
    ~DirectionalLight();

    void setLight(const glm::vec3& color, GLfloat ambientIntensity, const glm::vec3& diffuseDirection, GLfloat diffuseIntensity);
    void setMainShader(Shader* shader);
    void setShadowShader(Shader* shader);
    void setShadowMap(GLuint width, GLuint height, GLfloat near, GLfloat far);
    void updateLightProjectionView();

    void useShadow();
    void use();

    glm::vec3 color;
    GLfloat ambientIntensity;
    glm::vec3 diffuseDirection;
    GLfloat diffuseIntensity;
    Shader* mainShader;
    Shader* shadowShader;

    DirectionalShadowMap shadowMap;
    glm::mat4 lightProjectionView;
};