#pragma once

#include "Graphics/ShadowMaps/DirectionalShadowMap.h"
#include "Graphics/Shader.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct DirectionalLight {
    DirectionalLight();
    ~DirectionalLight();

    void setLight(const glm::vec3& color, GLfloat ambientIntensity, const glm::vec3& diffuseDirection, GLfloat diffuseIntensity);
    void setShadowShader(Shader* shader);
    void setShadowMap(GLuint width, GLuint height, GLfloat near, GLfloat far);
    void updateLightProjectionView();

    template<typename Func>
    void useShadow(Func&& renderStep);
    void use(Shader* shader);

    glm::vec3 color;
    GLfloat ambientIntensity;
    glm::vec3 diffuseDirection;
    GLfloat diffuseIntensity;
    Shader* shadowShader;

    DirectionalShadowMap shadowMap;
    glm::mat4 lightProjectionView;
};

#pragma massage("TODO: Remove opengl calls in directional light shadow map calculation")
template<typename Func>
void DirectionalLight::useShadow(Func&& renderStep) {
    shadowShader->use();
    shadowShader->setm4("directionalLightTransform", lightProjectionView);
    glViewport(0, 0, shadowMap.width, shadowMap.height);
    shadowMap.write();
    glClear(GL_DEPTH_BUFFER_BIT);
    renderStep(shadowShader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}