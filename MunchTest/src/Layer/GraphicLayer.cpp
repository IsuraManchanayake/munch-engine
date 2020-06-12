#include "Layer/GraphicLayer.h"
#include "Physics/Physics.h"
#include "Core/Logger.h"
#include "Core/Color.h"
#include "Core/V3.h"
#include "Graphics/Entities/EntityFactory.h"
#include "Graphics/ShaderFactory.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
GraphicLayer::GraphicLayer(std::string name, int width, int height)
    : Layer(std::move(name), width, height), 
    entities(), shaders(),
    defaultShader(nullptr), projection(), dirDebugShader(), 
    directionalLight(), 
    camera({1.f, 1.f, 1.f}, {0.f, 1.f, 0.f}, -90.f, 0.f, 2.f, 1.f), 
    keys() {
}

GraphicLayer::~GraphicLayer() {
    for (const auto& entity : entities) {
        delete entity;
    }
    for (const auto& shader : shaders) {
        delete shader;
    }
}

void GraphicLayer::setup() {
    camera = {V3::_111, V3::_010, -90.f, 0.f, 2.f, 1.f};

    const GLuint dirShadowMapWidth = 512, dirShadowMapHeight = 512;
    const GLfloat dirShadowMapNear = 0.1f, dirShadowMapFar = 40.0f;
    directionalLight.setLight(Color::white, 0.4f, {1.f, -1.f, 1.f}, 0.6f);
    directionalLight.setShadowMap(dirShadowMapWidth, dirShadowMapHeight, dirShadowMapNear, dirShadowMapFar);

    auto* sphere = EntityFactory::sphere();
    auto* plane = EntityFactory::terrain();
    glm::mat4 transform(1.f);
    transform = glm::translate(transform, { 0.f, 2.f, 0.f });
    transform = glm::scale(transform, { 100.f, 1.f, 100.f });
    transform = glm::rotate(transform, glm::radians(-90.0f), { 1.0f, 0.0f, 0.0f });
    transform = glm::translate(transform, { -0.5f, -0.5f, 0.0f });
    plane->transform = transform;
    entities.push_back(sphere);
    entities.push_back(plane);
    
    defaultShader = ShaderFactory::defaultShader();
    Shader* dirShadowShader = ShaderFactory::dirShadowShader();
    shaders.push_back(defaultShader);
    shaders.push_back(dirShadowShader);

    sphere->attachShader(defaultShader);
    plane->attachShader(defaultShader);
    directionalLight.setMainShader(defaultShader);
    directionalLight.setShadowShader(dirShadowShader);

    projection = glm::perspective(glm::radians(60.f), static_cast<float>(width) / height, 0.1f, 100.f);

    dirDebugShader.create("Shaders/debug_quad.vert.glsl", "Shaders/debug_quad.frag.glsl");
    dirDebugShader.use();
    dirDebugShader.seti1("depthMap", 0);
}

void GraphicLayer::update() {
    const float timef = Physics::currentTimeSeconds;
    camera.keyControl(keys);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.f, 0.f, 0.f, 1.f);

    directionalLight.useShadow();
    for(auto& entity : entities) {
        entity->render(directionalLight.shadowShader);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);   

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 view = camera.viewMat();
    
    defaultShader->use();
    directionalLight.use();
    defaultShader->setm4("projection", projection);
    defaultShader->setm4("view", view);
    defaultShader->setf3("eye", camera.position);
    defaultShader->seti1("pointLightCount", 0);
    defaultShader->seti1("spotLightCount", 0);

    for(auto& entity : entities) {
        entity->render();
    }    
    
    float debugWindowHeight = 200;
    float debugWindowWidth = (directionalLight.shadowMap.width * debugWindowHeight) / directionalLight.shadowMap.height;
    glViewport(0, 0, debugWindowWidth, debugWindowHeight);
    dirDebugShader.use();
    dirDebugShader.setf1("near_plane", directionalLight.shadowMap.near);
    dirDebugShader.setf1("far_plane", directionalLight.shadowMap.far);
    directionalLight.shadowMap.read(GL_TEXTURE0);
    renderQuad();
}

bool GraphicLayer::onMouseMove(MouseMoveEvent& event) {
    camera.mouseControl(event.dx, event.dy);
    return true;
}

bool GraphicLayer::onKeyPress(KeyPressEvent& event) {
    keys[event.key] = 1;
    return false;
}


bool GraphicLayer::onKeyRelease(KeyReleaseEvent& event) {
    keys[event.key] = 0;
    return false;
}


bool GraphicLayer::onKeyRepeat(KeyRepeatEvent& event) {
    keys[event.key]++;
    return false;
}

bool GraphicLayer::onWindowResize(WindowResizeEvent& event) {
    width = event.width;
    height = event.height;
    projection = glm::perspective(glm::radians(60.f), static_cast<float>(width) / height, 0.1f, 100.f);
    return false;
}
