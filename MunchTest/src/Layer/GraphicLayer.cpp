#include "Layer/GraphicLayer.h"
#include "Physics/Physics.h"
#include "Core/Logger.h"
#include "Core/Color.h"
#include "Core/V3.h"
#include "Graphics/Entities/EntityFactory.h"
#include "Graphics/ShaderFactory.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace detail {
    unsigned int quadVAO = 0;
    unsigned int quadVBO;
    void renderQuad();
}

GraphicLayer::GraphicLayer(std::string name, int width, int height)
    : Layer(std::move(name), width, height), 
    entities(), shaders(),
    defaultShader(nullptr), projection(), 
    directionalLight(), 
    camera({1.f, 1.f, 1.f}, {0.f, 1.f, 0.f}, -90.f, 0.f, 2.f, 1.f), 
    keys(), hdri(),
    dirDebugShader(){
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
    // Camera
    camera = {V3::_111, V3::_010, -90.f, 0.f, 2.f, 1.f};
    
    // Shaders
    defaultShader = ShaderFactory::defaultShader();
    Shader* dirShadowShader = ShaderFactory::dirShadowShader();
    Shader* omniDirShadowShader = ShaderFactory::omniDirShadowShader();
    shaders.push_back(defaultShader);
    shaders.push_back(dirShadowShader);
    shaders.push_back(omniDirShadowShader);

    // Directional light
    const GLuint dirShadowMapWidth = 512, dirShadowMapHeight = 512;
    const GLfloat dirShadowMapNear = 0.1f, dirShadowMapFar = 40.0f;
    directionalLight.setLight(Color::white, 0.4f, {1.f, -1.f, 1.f}, 0.6f);
    directionalLight.setShadowMap(dirShadowMapWidth, dirShadowMapHeight, dirShadowMapNear, dirShadowMapFar);
    directionalLight.setMainShader(defaultShader);
    directionalLight.setShadowShader(dirShadowShader);

    // Point lights and spot lights
    const GLuint omniShadowMapWidth = 512, omniShadowMapHeight = 512;
    const GLfloat omniShadowMapNear = 0.0f, omniShadowMapFar = 40.0f;
    
    pointLights.resize(2);
    pointLights[0].setPoint(0, Color::magenta, {  5.0f,  4.0f,  5.0f }, 0.3f * glm::vec3{ 1.0f, 1.0f, 1.0f }, 1.0f);
    pointLights[1].setPoint(1, Color::cyan, { -5.0f,  4.0f, -5.0f }, 0.3f * glm::vec3{ 1.0f, 1.0f, 1.0f }, 1.0f);
    for(auto& pointLight : pointLights) {
        pointLight.setShadowMap(omniShadowMapWidth, omniShadowMapHeight, omniShadowMapNear, omniShadowMapFar);
        pointLight.setMainShader(defaultShader);
        pointLight.setShadowShader(omniDirShadowShader);
    }
    spotLights.resize(2);
    spotLights[0].setSpot(0, Color::yellow, {  2.f,  2.f,  0.f }, { 0.f, -1.f, 0.f }, 0.4f * glm::vec3{ 1.0f, 1.0f, 1.0f }, 45.f, 1.0f);
    spotLights[1].setSpot(1, Color::green, { -2.f,  2.f,  0.f }, { 0.f, -1.f, 0.f }, 0.4f * glm::vec3{ 1.0f, 1.0f, 1.0f }, 45.f, 1.0f);
    for(auto& spotLight : spotLights) {
        spotLight.setShadowMap(omniShadowMapWidth, omniShadowMapHeight, omniShadowMapNear, omniShadowMapFar);
        spotLight.setMainShader(defaultShader);
        spotLight.setShadowShader(omniDirShadowShader);
    }

    // Entities
    auto* sphere = EntityFactory::sphere();
    auto* plane = EntityFactory::terrain();
    glm::mat4 transform(1.f);
    transform = glm::translate(transform, { 0.f, 2.f, 0.f });
    transform = glm::scale(transform, { 100.f, 1.f, 100.f });
    transform = glm::rotate(transform, glm::radians(-90.0f), { 1.0f, 0.0f, 0.0f });
    transform = glm::translate(transform, { -0.5f, -0.5f, 0.0f });
    plane->transform = transform;
    sphere->attachShader(defaultShader);
    plane->attachShader(defaultShader);
    entities.push_back(sphere);
    entities.push_back(plane);

    hdri.create("Textures/quarry_03.jpg");

    projection = glm::perspective(glm::radians(60.f), static_cast<float>(width) / height, 0.1f, 100.f);

    dirDebugShader.create("Shaders/debug_quad.vert.glsl", "Shaders/debug_quad.frag.glsl");
    dirDebugShader.use();
    dirDebugShader.seti1("depthMap", 0);
}

void GraphicLayer::update() {
    const float timef = Physics::currentTimeSeconds;
    
    // Updates
    camera.keyControl(keys);    
    spotLights[0].position = glm::vec3(2 * cos(timef / 3), 3.0f, 2 * sin(timef / 3));
    spotLights[0].updateLightProjectionView();
    spotLights[1].position = glm::vec3(-2 * cos(timef / 3), 3.0f, -2 * sin(timef / 3));
    spotLights[1].updateLightProjectionView();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.f, 0.f, 0.f, 1.f);

    directionalLight.useShadow([&](Shader* shader) {
        for(auto& entity : entities) {
            entity->render(shader);
        }
    });
    for(auto& pointLight: pointLights) {
        pointLight.useShadow([&](Shader* shader) {
            for(auto& entity : entities) {
                entity->render(shader);
            }   
        });
    }
    for(auto& spotLight: spotLights) {
        spotLight.useShadow([&](Shader* shader) {
            for(auto& entity : entities) {
                entity->render(shader);
            }   
        });
    }

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 view = camera.viewMat();
    
    hdri.render(view, projection);
    defaultShader->use();
    directionalLight.use();
    for(auto& pointLight: pointLights) {
        pointLight.use();
    }
    for(auto& spotLight: spotLights) {
        spotLight.use();
    }
    defaultShader->setm4("projection", projection);
    defaultShader->setm4("view", view);
    defaultShader->setf3("eye", camera.position);
    defaultShader->seti1("pointLightCount", 2);
    defaultShader->seti1("spotLightCount", 2);
    for(auto& entity : entities) {
        entity->render();
    }    
    
    // Debug
    float debugWindowHeight = 200;
    float debugWindowWidth = (directionalLight.shadowMap.width * debugWindowHeight) / directionalLight.shadowMap.height;
    glViewport(0, 0, debugWindowWidth, debugWindowHeight);
    dirDebugShader.use();
    dirDebugShader.setf1("near_plane", directionalLight.shadowMap.near);
    dirDebugShader.setf1("far_plane", directionalLight.shadowMap.far);
    directionalLight.shadowMap.read(GL_TEXTURE0);
    detail::renderQuad();
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

namespace detail {
    void renderQuad() {
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
}