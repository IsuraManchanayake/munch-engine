#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Material {
    Material();
    ~Material();

    void set(const glm::vec3& color, GLfloat shininess, GLfloat intensity,
                     GLfloat reflectivity, GLfloat translucency);
    void use(GLint specularColorLocation, GLint specularShininessLocation, GLint specularIntensityLocation, 
             GLint reflectivityLocation, GLint translucencyLocation);

    glm::vec3 specularColor;
    GLfloat specularShininess;
    GLfloat specularIntensity;
    GLfloat reflectivity;
    GLfloat translucency;
};