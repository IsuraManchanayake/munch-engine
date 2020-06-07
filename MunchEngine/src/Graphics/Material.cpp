#include "Graphics/Material.h"

Material::Material()
    : specularColor(1.f, 1.f, 1.f), specularShininess(16.f), specularIntensity(1.f),
      reflectivity(0.f), translucency(0.f) {
}

Material::~Material() {
}

void Material::set(const glm::vec3& color, GLfloat shininess, GLfloat intensity, GLfloat reflectivity, GLfloat translucency) {
    specularColor = color;
    specularShininess = shininess;
    specularIntensity = intensity;
    this->reflectivity = reflectivity;
    this->translucency = translucency;
}

void Material::use(GLint specularColorLocation, GLint specularShininessLocation, GLint specularIntensityLocation,
                   GLint reflectivityLocation, GLint translucencyLocation) {
    glUniform3f(specularColorLocation, specularColor.r, specularColor.g, specularColor.b);
    glUniform1f(specularShininessLocation, specularShininess);
    glUniform1f(specularIntensityLocation, specularIntensity);
    glUniform1f(reflectivityLocation, reflectivity);
    glUniform1f(translucencyLocation, translucency);
}