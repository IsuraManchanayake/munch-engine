#pragma once

#include "Graphics/Mesh.h"
#include "Graphics/Shader.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <array>

struct Guide {
    Guide();
    ~Guide();

    void create();
    void create(const glm::vec3& position);
    void setPosition(const glm::vec3& position);
    void render(const glm::mat4& viewM, const glm::mat4& projectionM);

    std::array<glm::mat4, 3> axesTransforms;
    Mesh cube;
    Shader shader;
    GLint uniformModelLocation;
    GLint uniformViewLocation;
    GLint uniformProjectionLocation;
    GLint uniformColorLocation;

    static bool enable;
};