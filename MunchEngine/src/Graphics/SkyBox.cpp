#include "Graphics/SkyBox.h"
#include "Core/Common.h"

SkyBox::SkyBox() : Resource(), mesh(), shader(), textureId(), averageColor() { 
}

SkyBox::~SkyBox() {
    clear();
}

void SkyBox::clear() {
    if(copied) return;
    if(textureId) {
        glDeleteTextures(1, &textureId);
    }
    textureId = 0;
}

void SkyBox::create(const std::array<std::string, 6>& facePaths) {
    shader.create("Shaders/skybox.vert.glsl", "Shaders/skybox.frag.glsl");

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    size_t rt = 0, gt = 0, bt = 0;
    size_t count = 0;
    for(size_t i = 0; i < 6; i++) {
        GLint width, height, bitDepth;
        unsigned char* data = stbi_load(facePaths[i].c_str(), &width, &height, &bitDepth, 0);
        if (!data) {
            std::cerr << "Failed to load texture data from " << facePaths[i] << "\n";
            error_exit(1);
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        
        for (unsigned p = 0, s = width * height; p < s; p++) {
            rt += data[3 * p + 0];
            gt += data[3 * p + 1];
            bt += data[3 * p + 2];
        }
        count += width * height;

        stbi_image_free(data);
    }
    averageColor = { rt / (255.f * count), gt / (255.f * count), bt / (255.f * count) };

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    unsigned indices[] = {
        0, 1, 2, 2, 1, 3,
        2, 3, 5, 5, 3, 7,
        5, 7, 4, 4, 7, 6,
        4, 6, 0, 0, 6, 1,
        4, 0, 5, 5, 0, 2,
        1, 6, 3, 3, 6, 7
    };

    float vertices[] = {
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

void SkyBox::render(const glm::mat4& view, const glm::mat4& projection) {
    glDepthMask(GL_FALSE);

    glm::mat4 translationRemovedView = glm::mat4(glm::mat3(view));

    shader.use();
    shader.setm4("view", translationRemovedView);
    shader.setm4("projection", projection);
    shader.seti1("skybox", 0);
    read(GL_TEXTURE0);
    mesh.render();

    glDepthMask(GL_TRUE);
}

void SkyBox::read(GLenum textureUnit) {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
}