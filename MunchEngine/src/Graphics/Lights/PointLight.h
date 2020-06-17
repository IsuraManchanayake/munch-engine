#pragma once

#include "Graphics/ShadowMaps/OmniDirectionalShadowMap.h"
#include "Graphics/Shader.h"
#include "Core/Common.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>

struct PointLight {
    PointLight();
    ~PointLight();

    void setPoint(unsigned index, const glm::vec3& color, const glm::vec3& position, const glm::vec3& constants, GLfloat intensity);
    void setShadowShader(Shader* shader);
    void setShadowMap(GLuint width, GLuint height, GLfloat near, GLfloat far);
    void updateLightProjectionView();

    template<typename Func>
    void useShadow(Func&& renderStep);
    void use(Shader* shader);
    
    glm::vec3 color;
    glm::vec3 position;
    glm::vec3 constants;
    GLfloat intensity;
    unsigned index;
    Shader* shadowShader;

    OmniDirectionalShadowMap shadowMap;
    std::array<glm::mat4, 6> lightTransforms;
};

#pragma massage("TODO: Remove opengl calls in directional light shadow map calculation")
template<typename Func>
void PointLight::useShadow(Func&& renderStep) {
    glViewport(0, 0, shadowMap.width, shadowMap.height);
    shadowMap.write();
    glClear(GL_DEPTH_BUFFER_BIT);
    shadowShader->use();
    for(size_t i = 0; i < 6; i++) {
        shadowShader->setm4(cat("lightM[", i, "]"), lightTransforms[i]);
    }
    shadowShader->setf3("lightPos", position);
    shadowShader->setf1("far", shadowMap.far);
    renderStep(shadowShader); 
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}