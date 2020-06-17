#include "Layer/PBRLayer.h"
#include "Physics/Physics.h"
#include "Core/Logger.h"
#include "Core/Color.h"
#include "Core/V3.h"
#include "Graphics/Entities/EntityFactory.h"
#include "Graphics/ShaderFactory.h"
#include "Graphics/Screenshot.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace detail2 {
    unsigned int quadVAO = 0;
    unsigned int quadVBO;
    void renderQuad();
}

PBRLayer::PBRLayer(std::string name, int width, int height)
    : Layer(std::move(name), width, height),
    entities(), terrains(), shaders(),
    defaultShader(nullptr), terrainShader(nullptr), projection(), 
    directionalLight(), 
    camera({1.f, 1.f, 1.f}, {0.f, 1.f, 0.f}, -90.f, 0.f, 2.f, 1.f), 
    keys(), hdri(),
    dirDebugShader(){
}

PBRLayer::~PBRLayer() {
    for (const auto& entity : entities) {
        delete entity;
    }
    for (const auto& shader : shaders) {
        delete shader;
    }
}

void PBRLayer::setup() {
    // Camera
    // camera = {{0.404f, 1.751f, 10.25f}, V3::_010, -106.87f, 14.47f, 2.f, 1.f};
    camera = {{-12.60f, 21.26f, -15.71f}, V3::_010, 68.15f, -42.24f, 2.f, 1.f};
    
    // Shaders
    defaultShader = ShaderFactory::pbrShader();
    terrainShader = ShaderFactory::terrainShader();
    shaders.push_back(defaultShader);
    shaders.push_back(terrainShader);
    Shader* dirShadowShader = ShaderFactory::dirShadowShader();
    Shader* omniDirShadowShader = ShaderFactory::omniDirShadowShader();
    shaders.push_back(dirShadowShader);
    shaders.push_back(omniDirShadowShader);

    // Directional light
    const GLuint dirShadowMapWidth = 800, dirShadowMapHeight = 800;
    const GLfloat dirShadowMapNear = 0.1f, dirShadowMapFar = 100.0f;
    directionalLight.setLight(Color::white, 0.5f, {1.f, -1.f, 1.f}, 4.f);
    directionalLight.setShadowMap(dirShadowMapWidth, dirShadowMapHeight, dirShadowMapNear, dirShadowMapFar);
    directionalLight.setShadowShader(dirShadowShader);

    // Point lights and spot lights
    const GLuint omniShadowMapWidth = 512, omniShadowMapHeight = 512;
    const GLfloat omniShadowMapNear = 0.0f, omniShadowMapFar = 40.0f;
    
    pointLights.resize(2);
    pointLights[0].setPoint(0, Color::yellow, {  5.0f,  4.0f,  5.0f }, 0.4f * glm::vec3{ 1.0f, 1.0f, 1.0f }, 10.0f);
    pointLights[1].setPoint(1, Color::cyan, { -5.0f,  4.0f, -5.0f }, 0.4f * glm::vec3{ 1.0f, 1.0f, 1.0f }, 10.0f);
    for(auto& pointLight : pointLights) {
        pointLight.setShadowMap(omniShadowMapWidth, omniShadowMapHeight, omniShadowMapNear, omniShadowMapFar);
        pointLight.setShadowShader(omniDirShadowShader);
    }
    spotLights.resize(2);
    spotLights[0].setSpot(0, Color::yellow, {  2.f,  2.f,  0.f }, { 0.f, -1.f, 0.f }, 0.4f * glm::vec3{ 1.0f, 1.0f, 1.0f }, 45.f, 1.0f);
    spotLights[1].setSpot(1, Color::green, { -2.f,  2.f,  0.f }, { 0.f, -1.f, 0.f }, 0.4f * glm::vec3{ 1.0f, 1.0f, 1.0f }, 45.f, 1.0f);
    for(auto& spotLight : spotLights) {
        spotLight.setShadowMap(omniShadowMapWidth, omniShadowMapHeight, omniShadowMapNear, omniShadowMapFar);
        spotLight.setShadowShader(omniDirShadowShader);
    }

    // Entities
    auto* terrain = EntityFactory::terrain("Textures/heightMap.png"); {
        glm::mat4 transform(1.f);
        transform = glm::translate(transform, { 0.f, 0.f, 0.f });
        transform = glm::scale(transform, { 1000.f, 1.f, 1000.f });
        transform = glm::rotate(transform, glm::radians(-90.0f), { 1.0f, 0.0f, 0.0f });
        transform = glm::translate(transform, { -0.5f, -0.5f, 0.0f });
        terrain->transform = transform;
        // terrain->heightMap = Texture::createImageTexture("Textures/heightMap.png");
        terrain->uvScale = {10.f, 10.f};
        terrain->model->materials[0].albedo = Texture::createImageTexture("Textures/aerial_grass_rock_diff_2k.jpg");
        terrain->model->materials[0].normal = Texture::createImageTexture("Textures/aerial_grass_rock_nor_2k.jpg");
        terrain->model->materials[0].roughness = Texture::createImageTexture("Textures/aerial_grass_rock_rough_2k.jpg");
        terrain->model->materials[0].ao = Texture::createImageTexture("Textures/aerial_grass_rock_ao_2k.jpg");

        // plane->model->materials[0].attachRoughness(0.6f);
        terrain->model->materials[0].attachMetallic(0.9f);
    }
    terrain->attachShader(terrainShader);
    terrains.push_back(terrain);

    for(size_t j = 0, M = 5; j < M; j++) {
        for(size_t i = 0, N = 5; i < N; i++) {
            auto* sphere = EntityFactory::sphere(); {
                glm::mat4 transform(1.f);
                transform = glm::translate(transform, 17.f * V3::_010 + 7.5f * (i - (N - 1) / 2.f) * V3::_100 + 7.5f * (j - (M - 1) / 2.f) * V3::_001);
                transform = glm::scale(transform, 3.f * V3::_111);
                transform = glm::rotate(transform, glm::radians(-90.f), V3::_100);
                sphere->transform = transform;

                float factori = static_cast<float>(i) / (N - 1);
                float factorj = static_cast<float>(j) / (M - 1);
                sphere->model->materials[0].attachColor(TextureMapType::albedo, Color::hsv(50, 100, 100));
                // sphere->model->materials[0].attachRoughness(factorj);
                sphere->model->materials[0].roughness = Texture::createImageTexture("Textures/metal_plate_rough_2k.jpg");
                // sphere->model->materials[0].metallic = Texture::createImageTexture("Textures/metal_plate_spec_2k.jpg");
                sphere->model->materials[0].attachMetallic(factori);
                // sphere->model->materials[0].attachRoughness(factor);

                // sphere->model->materials[0].attachSpecular(1.0f);
                // sphere->model->materials[0].attachGloss(50.f);
                // sphere->model->materials[0].normal = Texture::createImageTexture("Textures/rusty_metal_02_nor_2k.jpg");
                // sphere->model->materials[0].albedo = Texture::createImageTexture("Textures/rusty_metal_02_diff_2k.jpg");
                sphere->model->materials[0].normal = Texture::createImageTexture("Textures/metal_plate_Nor_2k.jpg");
                sphere->model->materials[0].albedo = Texture::createImageTexture("Textures/metal_plate_diff_2k.jpg");
                sphere->model->materials[0].ao = Texture::createImageTexture("Textures/metal_plate_AO_2k.jpg");
            }
            sphere->attachShader(defaultShader);
            entities.push_back(sphere);
        }
    }

    // hdri.create("Textures/quarry_03_8k.jpg");
    // hdri.createHDR("Textures/the_lost_city_4k.hdr");
    // hdri.createHDR("Textures/small_rural_road_4k.hdr");
    hdri.createHDR("Textures/cape_hill_4k.hdr");
    // hdri.createHDR("Textures/old_bus_depot_4k.hdr");
    // hdri.createHDR("Textures/newport_loft.hdr");
    hdri.setIrradianceMap();
    // plane->model->materials[0].albedo = hdri.irradiance.texture;

    projection = glm::perspective(glm::radians(60.f), static_cast<float>(width) / height, 0.1f, 500.f);

    // dirDebugShader.create("Shaders/debug_quad.vert.glsl", "Shaders/debug_quad.frag.glsl");
    // dirDebugShader.use();
    // dirDebugShader.seti1("depthMap", 0);
}

void PBRLayer::update() {
    const float timef = Physics::currentTimeSeconds;
    
    // Updates
    camera.keyControl(keys);    
    pointLights[0].position = glm::vec3(5.5 * cos(timef / 3), 7.0f, 5.5 * sin(timef / 3));
    pointLights[0].updateLightProjectionView();
    pointLights[1].position = glm::vec3(-5.5 * cos(timef / 3), 7.0f, -5.5 * sin(timef / 3));
    pointLights[1].updateLightProjectionView();
    // spotLights[0].position = glm::vec3(4 * cos(timef / 3), 3.0f, 4 * sin(timef / 3));
    // spotLights[0].updateLightProjectionView();
    // spotLights[1].position = glm::vec3(-4 * cos(timef / 3), 3.0f, -4 * sin(timef / 3));
    // spotLights[1].updateLightProjectionView();

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
    directionalLight.use(defaultShader);
    for(auto& pointLight: pointLights) {
        pointLight.use(defaultShader);
    }
    for(auto& spotLight: spotLights) {
        spotLight.use(defaultShader);
    }
    defaultShader->setm4("projection", projection);
    defaultShader->setm4("view", view);
    defaultShader->setf3("eye", camera.position);
    defaultShader->seti1("pointLightCount", 2);
    defaultShader->seti1("spotLightCount", 2);
    hdri.useIrradianceMap(defaultShader, "irradianceMap");
    for(auto& entity : entities) {
        entity->render();
    }

    
    terrainShader->use();
    directionalLight.use(terrainShader);
    for(auto& pointLight: pointLights) {
        pointLight.use(terrainShader);
    }
    for(auto& spotLight: spotLights) {
        spotLight.use(terrainShader);
    }
    terrainShader->setm4("projection", projection);
    terrainShader->setm4("view", view);
    terrainShader->setf3("eye", camera.position);
    terrainShader->seti1("pointLightCount", 2);
    terrainShader->seti1("spotLightCount", 2);
    hdri.useIrradianceMap(terrainShader, "irradianceMap");
    for(auto& terrain : terrains) {
        terrain->render();
    }
    
    // Debug
    // float debugWindowHeight = 200;
    // float debugWindowWidth = (directionalLight.shadowMap.width * debugWindowHeight) / directionalLight.shadowMap.height;
    // glViewport(0, 0, debugWindowWidth, debugWindowHeight);
    // dirDebugShader.use();
    // dirDebugShader.setf1("near_plane", directionalLight.shadowMap.near);
    // dirDebugShader.setf1("far_plane", directionalLight.shadowMap.far);
    // directionalLight.shadowMap.read(GL_TEXTURE0);
    // detail2::renderQuad();
}

bool PBRLayer::onMouseMove(MouseMoveEvent& event) {
    camera.mouseControl(event.dx, event.dy);
    return true;
}

bool PBRLayer::onKeyPress(KeyPressEvent& event) {
    keys[event.key] = 1;
    if(event.key == GLFW_KEY_P) {
        Screenshot::capturepng("screenshot_$time.png", width, height);
    }
    return false;
}

bool PBRLayer::onKeyRelease(KeyReleaseEvent& event) {
    keys[event.key] = 0;
    return false;
}

bool PBRLayer::onKeyRepeat(KeyRepeatEvent& event) {
    keys[event.key]++;
    return false;
}

bool PBRLayer::onWindowResize(WindowResizeEvent& event) {
    width = event.width;
    height = event.height;
    projection = glm::perspective(glm::radians(60.f), static_cast<float>(width) / height, 0.1f, 100.f);
    return false;
}

namespace detail2 {
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