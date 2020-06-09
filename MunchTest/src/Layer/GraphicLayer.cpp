#include "Layer/GraphicLayer.h"
#include "Physics/Physics.h"
#include "Core/Color.h"
#include "Core/Logger.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

GraphicLayer::GraphicLayer(std::string name, int width, int height)
    : Layer(std::move(name), width, height), scene(), keys() {
}

void GraphicLayer::setup() {
    {
        Shader mainShader;
        mainShader.create("Shaders/moving.vert.glsl", "Shaders/moving.frag.glsl");
        scene.setMainShader(mainShader);



        Material plasticMaterial;
        plasticMaterial.set({1.0f, 1.0f, 1.0f,}, 16.0f, 1.0f, 0.f, 0.f);
        // Material glassMaterial;
        // glassMaterial.set({1.0f, 1.0f, 1.0f,}, 16.0f, 1.0f, 0.f, 0.f);

        // 9 Spheres
        // for (int i = -1; i <= 1; i++) {
        //     for (int j = -1; j <= 1; j++) {
        //         glm::mat4 transform(1.f);
        //         transform = glm::translate(transform, { 3.f * i, 1.f, 3.f * j });
        //         scene.addModel(Model::sphere(transform, glassMaterial));
        //     }
        // }

        // Single sphere
        for (int i = 0; i <= 0; i++) {
            for (int j = 0; j <= 0; j++) {
                glm::mat4 transform(1.f);
                transform = glm::translate(transform, { 3.f * i, 0.f, 3.f * j });
                scene.addModel(Model::sphere(transform, plasticMaterial));
            }
        }
        
        // External Model
        // for (int i = 0; i <= 0; i++) {
        //     for (int j = 0; j <= 0; j++) {
        //         glm::mat4 transform(1.f);
        //         //transform = glm::translate(transform, { 3.f * i, 0.f, 3.f * j });
        //         transform = glm::scale(transform, 0.05f * glm::vec3{1.f, 1.f, 1.f});
        //         //transform = glm::rotate(transform, glm::radians(-90.f), {1.f, 0.f, 0.f});
        //         //scene.addModel("Models/african_head.obj", transform, glassMaterial);
        //         scene.addModel("Models/sponza.obj", transform, glassMaterial);
        //     }
        // }

        glm::mat4 transform(1.f);
        transform = glm::scale(transform, {100.f, 1.f, 100.f });
        transform = glm::rotate(transform, glm::radians(-90.0f), {1.0f, 0.0f, 0.0f});
        transform = glm::translate(transform, {-0.5f, -0.5f, 0.0f});
        scene.addModel(Model::terrain(transform, plasticMaterial));

        Camera camera(glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.f, 1.f, 0.f), -90.f, 0.f, 2.f, 1.f);
        scene.setCamera(camera);

        const GLuint dirShadowMapWidth = 512, dirShadowMapHeight = 512;
        const GLfloat dirShadowMapNear = 0.1f, dirShadowMapFar = 40.0f;

        DirectionalLight directionalLight;
        directionalLight.setAmbient(Color::white, 0.2f);
        directionalLight.setDiffuse(Color::white, {-1.0f, -1.0f, -1.0f}, 0.4f);
        directionalLight.setShadowMap(dirShadowMapWidth, dirShadowMapHeight, dirShadowMapNear, dirShadowMapFar);
        scene.setDirectionalLight(directionalLight);

        const GLuint omniShadowMapWidth = 512, omniShadowMapHeight = 512;
        const GLfloat omniShadowMapNear = 0.0f, omniShadowMapFar = 40.0f;

        std::array<PointLight, 2> pointLights;
        static_assert(pointLights.size() <= Scene::maxPointLights, "");
        pointLights[0].setPoint(Color::magenta, {  5.0f,  5.0f,  5.0f }, 0.1f * glm::vec3{ 1.0f, 1.0f, 1.0f }, 1.0f);
        pointLights[1].setPoint(Color::cyan, { -5.0f,  5.0f, -5.0f }, 0.1f * glm::vec3{ 1.0f, 1.0f, 1.0f }, 1.0f);
        for(auto& pointLight : pointLights) {
            pointLight.setShadowMap(omniShadowMapWidth, omniShadowMapHeight, omniShadowMapNear, omniShadowMapFar);
        }
        for(const auto& pointLight : pointLights) {
            scene.addPointLight(pointLight);
        }

        std::array<SpotLight, 2> spotLights;
        static_assert(spotLights.size() <= Scene::maxSpotLights, "");
        spotLights[0].setSpot(Color::yellow, {  2.f,  2.f,  0.f }, { 0.f, -1.f, 0.f }, 0.1f * glm::vec3{ 1.0f, 1.0f, 1.0f }, 45.f, 1.0f);
        spotLights[1].setSpot(Color::green, { -2.f,  2.f,  0.f }, { 0.f, -1.f, 0.f }, 0.1f * glm::vec3{ 1.0f, 1.0f, 1.0f }, 45.f, 1.0f);
        for(auto& spotLight : spotLights) {
            spotLight.setShadowMap(omniShadowMapWidth, omniShadowMapHeight, omniShadowMapNear, omniShadowMapFar);
        }
        for(const auto& spotLight : spotLights) {
            scene.addSpotLight(spotLight);
        }

        SkyBox skyBox;
        // std::array<std::string, 6> images = {
        //     "SkyBoxes/cupertin-lake_rt.tga",
        //     "SkyBoxes/cupertin-lake_lf.tga",
        //     "SkyBoxes/cupertin-lake_up.tga",
        //     "SkyBoxes/cupertin-lake_dn.tga",
        //     "SkyBoxes/cupertin-lake_bk.tga",
        //     "SkyBoxes/cupertin-lake_ft.tga"
        // };
        std::array<std::string, 6> images = {
            "SkyBoxes/cupertin-lake_rt.tga",
            "SkyBoxes/cupertin-lake_lf.tga",
            "SkyBoxes/cupertin-lake_up.tga",
            "SkyBoxes/cupertin-lake_dn.tga",
            "SkyBoxes/cupertin-lake_bk.tga",
            "SkyBoxes/cupertin-lake_ft.tga"
        };
        skyBox.create(images);
        // scene.directionalLight.ambientColor = skyBox.averageColor;
        scene.setSkyBox(skyBox);

        glm::mat4 projection = glm::perspective(glm::radians(60.f), static_cast<float>(width) / height, 0.1f, 100.f);
        scene.setProjection(projection);
    }

    scene.setAllUniforms();
    scene.setTextureUnits();

    // -------- Setup shadow map shaders
    scene.dirShadowShader.create("Shaders/directional_shadow_map.vert.glsl", "Shaders/directional_shadow_map.frag.glsl");

    scene.omniShadowShader.create("Shaders/omni_shadow_map.vert.glsl", "Shaders/omni_shadow_map.geom.glsl", "Shaders/omni_shadow_map.frag.glsl");
    // std::array<GLint, 6> uniformLights;
    // for(size_t i = 0; i < 6; i++) {
    //     uniformLights[i] = scene.omniShadowShader.getUniformLocation(cat("lightM[", i, "]"));
    // }
    
    // ---------- Debug shaders
    // Shader dirDebugShader;
    // dirDebugShader.create("Shaders/debug_quad.vert.glsl", "Shaders/debug_quad.frag.glsl");
    // dirDebugShader.use();
    // dirDebugShader.seti1("depthMap", 31);

    // Guide guide;
    // Guide guidePointLight;
    // Guide guideSpotLight;
    // guide.create({ 0.f, 0.f, 0.f });
    // guidePointLight.create();
    // guideSpotLight.create();
}

void GraphicLayer::update() {
    const float timef = Physics::currentTimeSeconds;
    scene.camera.keyControl(keys);

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // -------- Animations
    scene.spotLights[0].position = glm::vec3(2 * cos(timef / 3), 3.0f, 2 * sin(timef / 3));
    scene.spotLights[0].updateLightProjectionView();
    scene.spotLights[1].position = glm::vec3(-2 * cos(timef / 3), 3.0f, -2 * sin(timef / 3));
    scene.spotLights[1].updateLightProjectionView();

    // -------- Directional shadow map pass
    scene.dirShadowShader.use();
    scene.dirShadowShader.setm4("directionalLightTransform", scene.directionalLight.lightProjectionView);
    glViewport(0, 0, scene.directionalLight.map.width, scene.directionalLight.map.height);
    scene.directionalLight.map.write();
    glClear(GL_DEPTH_BUFFER_BIT);

    scene.render(scene.dirShadowShader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // -------- Omni directional shadow map pass
    for(auto& pointLight: scene.pointLights) {
        glViewport(0, 0, pointLight.map.width, pointLight.map.height);
        pointLight.map.write();
        glClear(GL_DEPTH_BUFFER_BIT);
        scene.omniShadowShader.use();

        for(size_t i = 0; i < 6; i++) {
            scene.omniShadowShader.setm4(cat("lightM[", i, "]"), pointLight.lightTransforms[i]);
        }
        scene.omniShadowShader.setf3("lightPos", pointLight.position);
        scene.omniShadowShader.setf1("far", pointLight.map.far);
        scene.omniShadowShader.validate();
        scene.render(scene.omniShadowShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    for(auto& spotLight: scene.spotLights) {
        glViewport(0, 0, spotLight.map.width, spotLight.map.height);
        spotLight.map.write();
        glClear(GL_DEPTH_BUFFER_BIT);
        scene.omniShadowShader.use();

        for(size_t i = 0; i < 6; i++) {
            scene.omniShadowShader.setm4(cat("lightM[", i, "]"), spotLight.lightTransforms[i]);
        }
        scene.omniShadowShader.setf3("lightPos", spotLight.position);
        scene.omniShadowShader.setf1("far", spotLight.map.far);
        scene.omniShadowShader.validate();
        scene.render(scene.omniShadowShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    // -------- Render pass
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    scene.render();

    // -------- Guide rendering
    // guide.setPosition(pointLights[0].position);
    // guidePointLight.setPosition(pointLights[0].position);
    //guideSpotLight.setPosition(spotLights[0].position);
    //guide.render(viewM, projectionM);
    // guidePointLight.render(viewM, projectionM);
    //guideSpotLight.render(viewM, projectionM);

    // --------- Debug pass
    // float debugWindowHeight = 200;
    // float debugWindowWidth = (scene.directionalLight.map.width * debugWindowHeight) / scene.directionalLight.map.height;
    // glViewport(0, 0, debugWindowWidth, debugWindowHeight);
    // dirDebugShader.use();
    // dirDebugShader.setf1("near_plane", scene.directionalLight.map.near);
    // dirDebugShader.setf1("far_plane", scene.directionalLight.map.far);
    // scene.directionalLight.map.read(GL_TEXTURE31);
    // renderQuad();
}

bool GraphicLayer::onMouseMove(MouseMoveEvent& event) {
    scene.camera.mouseControl(event.dx, event.dy);
    return false;
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
    glm::mat4 projection = glm::perspective(glm::radians(60.f), static_cast<float>(width) / height, 0.1f, 100.f);
    scene.setProjection(projection);
    return false;
}
