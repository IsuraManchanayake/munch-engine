#include "Graphics/HDRI.h"
#include "Graphics/stb_image.h"
#include "Core/Common.h"
#include "Core/Logger.h"

HDRI::HDRI() 
    : mesh(), shader(), texture(), textureLocation() {
}

HDRI::~HDRI() {
}

void HDRI::create(const std::string& path) {
    shader.create("Shaders/hdri.vert.glsl", "Shaders/hdri.frag.glsl");
    texture.create(path);

    shader.use();
    textureLocation = 0;
    shader.seti1("hdri", textureLocation);

    static const unsigned indices[] = {
        0, 1, 2, 2, 1, 3,
        2, 3, 5, 5, 3, 7,
        5, 7, 4, 4, 7, 6,
        4, 6, 0, 0, 6, 1,
        4, 0, 5, 5, 0, 2,
        1, 6, 3, 3, 6, 7
    };
    static const float vertices[] = {
        -1.0f,  1.0f, -1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, -1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
        -1.0f,  1.0f,  1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
         1.0f,  1.0f,  1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
        -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
         1.0f, -1.0f,  1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f
    };
    mesh.create(vertices, indices, size(vertices), size(indices));
}

void HDRI::render(const glm::mat4& view, const glm::mat4& projection) {
    glDepthMask(GL_FALSE);

    glm::mat4 translationRemovedView = glm::mat4(glm::mat3(view));

    shader.use();
    shader.setm4("view", translationRemovedView);
    shader.setm4("projection", projection);
    read(GL_TEXTURE0 + textureLocation);
    mesh.render();

    glDepthMask(GL_TRUE);
}

void HDRI::read(GLenum textureUnit) {
    texture.use(textureUnit);
}
