#pragma once

#include "Graphics/Mesh.h"
#include "Graphics/Shader.h"
#include "Graphics/Resource.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <array>
#include <iostream>
#include <string>

struct SkyBox : Resource {
    SkyBox();
    ~SkyBox();

    void clear();

    ADD_SPECIAL_CLASS_FUNCTIONS(SkyBox);

    void create(const std::array<std::string, 6>& facePaths);
    void render(const glm::mat4& view, const glm::mat4& projection);
    void read(GLenum textureUnit);

    Mesh mesh;
    Shader shader;
    GLuint textureId;
    glm::vec3 averageColor;
};