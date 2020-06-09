#include "Guide.h"

#include <glm/gtc/type_ptr.hpp>

bool Guide::enable = true;

Guide::Guide()
    : axesTransforms(), cube(), shader(), 
      uniformModelLocation(), uniformViewLocation(), uniformProjectionLocation(), uniformColorLocation() {
}

Guide::~Guide() {
    shader.clear();
    cube.clear();
}

void Guide::create() {
    create({ 0.f, 0.f, 0.f });
}

void Guide::create(const glm::vec3& position) {
    setPosition(position);

    cube.createCube();

    shader.create("Shaders/guide.vert.glsl", "Shaders/guide.frag.glsl");
    uniformModelLocation = shader.getUniformLocation("modelM");
    uniformViewLocation = shader.getUniformLocation("viewM");
    uniformProjectionLocation = shader.getUniformLocation("projectionM");
    uniformColorLocation = shader.getUniformLocation("inColor");
}

void Guide::setPosition(const glm::vec3& position) {
    for (auto& axesTransform : axesTransforms) {
        axesTransform = glm::mat4(1.f);
    }

    static const float l = 1.0f;
    static const float s = 0.01f;

    for (auto& axesTransform : axesTransforms) {
        axesTransform = glm::translate(axesTransform, position);
    }
    axesTransforms[0] = glm::scale(axesTransforms[0], { l, s, s });
    axesTransforms[1] = glm::scale(axesTransforms[1], { s, l, s });
    axesTransforms[2] = glm::scale(axesTransforms[2], { s, s, l });
}

void Guide::render(const glm::mat4& viewM, const glm::mat4& projectionM) {
    if (!enable) return;

    shader.use();

    glUniformMatrix4fv(uniformViewLocation, 1, GL_FALSE, glm::value_ptr(viewM));
    glUniformMatrix4fv(uniformProjectionLocation, 1, GL_FALSE, glm::value_ptr(projectionM));

    glUniformMatrix4fv(uniformModelLocation, 1, GL_FALSE, glm::value_ptr(axesTransforms[0]));
    glUniform3f(uniformColorLocation, 1.f, 0.f, 0.f);
    cube.render();
    glUniformMatrix4fv(uniformModelLocation, 1, GL_FALSE, glm::value_ptr(axesTransforms[1]));
    glUniform3f(uniformColorLocation, 0.f, 1.f, 0.f);
    cube.render();
    glUniformMatrix4fv(uniformModelLocation, 1, GL_FALSE, glm::value_ptr(axesTransforms[2]));
    glUniform3f(uniformColorLocation, 0.f, 0.f, 1.f);
    cube.render();
}