#include "Graphics/Scene.h"
#include "Core/Common.h"
#include "Core/Logger.h"

Scene::Scene() 
    : models(), camera({1.f, 1.f, 1.f}, {0.f, 1.f, 0.f}, -90.f, 0.f, 2.f, 1.f), 
      skyBox(), hdri(), mainShader(), projection(1.0f), 
      directionalLight(), pointLights(), spotLights(), reservedTextureUnits(),
      mainShaderInited(false), dirShadowShader(), omniShadowShader(), guide(),
      // uniforms
      directionalLightLocation(), pointLightLocations(), spotLightLocations(),
      materialLocation(), modelLocation(), projectionLocation(), viewLocation(),
      directionalLightTransformLocation(), textureLocation(), useNormalMapLocation(),
      normalMapLocation(), useDisplaceMapLocation(), displaceMapLocation(),
      pointLightCountLocation(), spotLightCountLocation(), eyeLocation() {
}

Scene::~Scene() {
}

void Scene::addModel(const std::string& path, const glm::mat4& transformation, const Material& material) {
    models.emplace_back(path, transformation, material);
}

void Scene::addModel(const Model& model) {
    models.push_back(model);
}

void Scene::setCamera(const Camera& camera) {
    this->camera = camera;
}

void Scene::setSkyBox(const SkyBox& skyBox) {
    this->skyBox = skyBox;
}

void Scene::setHDRI(const HDRI& hdri) {
    this->hdri = hdri;
}

void Scene::setGuide(const Guide& guide) {
    this->guide = guide;
}

void Scene::setMainShader(const Shader& shader) {
    this->mainShader = shader;
    mainShaderInited = true;
}

void Scene::setProjection(const glm::mat4& projection) {
    this->projection = projection;
}

void Scene::setDirectionalLight(const DirectionalLight& directionalLight) {
    this->directionalLight = directionalLight;
}

void Scene::addPointLight(const PointLight& pointLight) {
    this->pointLights.push_back(pointLight);
}

void Scene::addSpotLight(const SpotLight& spotLight) {
    this->spotLights.push_back(spotLight);
}

void Scene::setAllUniforms() {
    setCommonUniforms();
    setPointSpotLightUniforms();
}

void Scene::setCommonUniforms() {
    // Transformation locations
    modelLocation = mainShader.getUniformLocation("model");
    projectionLocation = mainShader.getUniformLocation("projection");
    viewLocation = mainShader.getUniformLocation("view");
    directionalLightTransformLocation = mainShader.getUniformLocation("directionalLightTransform");

    // Material locations
    materialLocation.specularColor = mainShader.getUniformLocation("material.specularColor");
    materialLocation.specularShininess = mainShader.getUniformLocation("material.specularShininess");
    materialLocation.specularIntensity = mainShader.getUniformLocation("material.specularIntensity");
    materialLocation.reflectivity = mainShader.getUniformLocation("material.reflectivity");
    materialLocation.translucency = mainShader.getUniformLocation("material.translucency");

    // DirectionalLight locations
    directionalLightLocation.ambientColor = mainShader.getUniformLocation("directionalLight.ambientColor");
    directionalLightLocation.ambientIntensity = mainShader.getUniformLocation("directionalLight.ambientIntensity");
    directionalLightLocation.diffuseColor = mainShader.getUniformLocation("directionalLight.diffuseColor");
    directionalLightLocation.diffuseDirection = mainShader.getUniformLocation("directionalLight.diffuseDirection");
    directionalLightLocation.diffuseIntensity = mainShader.getUniformLocation("directionalLight.diffuseIntensity");
    directionalLightLocation.shadowMap = mainShader.getUniformLocation("directionalLight.shadowMap");
    
    // Texture locations
    textureLocation = mainShader.getUniformLocation("theTexture");
    useNormalMapLocation = mainShader.getUniformLocation("useNormalMap");
    normalMapLocation = mainShader.getUniformLocation("normalMap");
    useDisplaceMapLocation = mainShader.getUniformLocation("useDisplaceMap");
    displaceMapLocation = mainShader.getUniformLocation("displaceMap");

    // lightcount locations
    pointLightCountLocation = mainShader.getUniformLocation("pointLightCount");
    spotLightCountLocation = mainShader.getUniformLocation("spotLightCount");

    // Camera locations
    eyeLocation = mainShader.getUniformLocation("eye");
}

void Scene::setPointSpotLightUniforms() {
    std::vector<PointLightLocation>().swap(pointLightLocations);
    for(size_t i = 0; i < pointLights.size(); i++) {
        PointLightLocation pointlightLocation;
        pointlightLocation.color = mainShader.getUniformLocation(cat("pointLights[", i, "].color"));
        pointlightLocation.position = mainShader.getUniformLocation(cat("pointLights[", i, "].position"));
        pointlightLocation.constants = mainShader.getUniformLocation(cat("pointLights[", i, "].constants"));
        pointlightLocation.intensity = mainShader.getUniformLocation(cat("pointLights[", i, "].intensity"));
        pointlightLocation.shadowMap = mainShader.getUniformLocation(cat("pointLights[", i, "].shadowMap"));
        pointlightLocation.shadowFar = mainShader.getUniformLocation(cat("pointLights[", i, "].shadowFar"));
        pointLightLocations.push_back(pointlightLocation);
    }
    std::vector<SpotLightLocation>().swap(spotLightLocations);
    for(size_t i = 0; i < spotLights.size(); i++) {
        SpotLightLocation spotlightLocation;
        spotlightLocation.color = mainShader.getUniformLocation(cat("spotLights[", i, "].color"));
        spotlightLocation.position = mainShader.getUniformLocation(cat("spotLights[", i, "].position"));
        spotlightLocation.direction = mainShader.getUniformLocation(cat("spotLights[", i, "].direction"));
        spotlightLocation.constants = mainShader.getUniformLocation(cat("spotLights[", i, "].constants"));
        spotlightLocation.angle = mainShader.getUniformLocation(cat("spotLights[", i, "].angle"));
        spotlightLocation.intensity = mainShader.getUniformLocation(cat("spotLights[", i, "].intensity"));
        spotlightLocation.shadowMap = mainShader.getUniformLocation(cat("spotLights[", i, "].shadowMap"));
        spotlightLocation.shadowFar = mainShader.getUniformLocation(cat("spotLights[", i, "].shadowFar"));
        spotLightLocations.push_back(spotlightLocation);
    }
}

void Scene::setTextureUnits() {
    mainShader.use();
    mainShader.seti1(textureLocation, 0);
    mainShader.seti1(directionalLightLocation.shadowMap, 1);
    for(size_t i = 0; i < maxPointLights; i++) {
        mainShader.seti1(cat("pointLights[", i, "].shadowMap"), i + 2);
    }
    for(size_t i = 0; i < maxSpotLights; i++) {
        mainShader.seti1(cat("spotLights[", i, "].shadowMap"), i + maxPointLights + 2);
    }
    mainShader.seti1("skyBox", maxPointLights + maxSpotLights + 2);
    mainShader.seti1(normalMapLocation, maxPointLights + maxSpotLights + 3);
    mainShader.seti1(displaceMapLocation, maxPointLights + maxSpotLights + 4);
    reservedTextureUnits = maxPointLights + maxSpotLights + 5;
}

void Scene::render(Shader& shader) {
    shader.use();
    for(auto& model : models) {
        shader.setm4("model", model.transform);
        for(auto& mesh : model.object->meshes) {
            mesh.render();
        }
    }
}

void Scene::render() {
    glm::mat4 view = camera.viewMat();
    // skyBox.render(view, projection);
    hdri.render(view, projection);

    mainShader.use();
    Shader::setm4(projectionLocation, projection);
    Shader::setm4(viewLocation, view);
    Shader::setm4(directionalLightTransformLocation, directionalLight.lightProjectionView);
    Shader::setf3(eyeLocation, camera.position);
    Shader::seti1(pointLightCountLocation, pointLights.size());
    Shader::seti1(spotLightCountLocation, spotLights.size());

    directionalLight.map.read(GL_TEXTURE1);
    directionalLight.use(directionalLightLocation.ambientColor, directionalLightLocation.ambientIntensity,
                         directionalLightLocation.diffuseColor, directionalLightLocation.diffuseDirection, 
                         directionalLightLocation.diffuseIntensity);
    for(size_t i = 0; i < pointLights.size(); i++) {
        pointLights[i].map.read(GL_TEXTURE2 + i);
        Shader::setf1(pointLightLocations[i].shadowFar, pointLights[i].map.far);
        pointLights[i].use(pointLightLocations[i].color, pointLightLocations[i].position, pointLightLocations[i].constants, 
                           pointLightLocations[i].intensity);
    }
    for(size_t i = 0; i < spotLights.size(); i++) {
        spotLights[i].map.read(GL_TEXTURE2 + maxPointLights + i);
        Shader::setf1(spotLightLocations[i].shadowFar, spotLights[i].map.far);
        spotLights[i].use(spotLightLocations[i].color, spotLightLocations[i].position, spotLightLocations[i].direction, 
                          spotLightLocations[i].constants, spotLightLocations[i].angle, spotLightLocations[i].intensity);
    }

    skyBox.read(GL_TEXTURE2 + maxPointLights + maxSpotLights);
    for(auto& model : models) {
        auto& material = model.material;
        auto& transform = model.transform;
        auto& object = model.object;
        Shader::setm4(modelLocation, transform);
        material.use(materialLocation.specularColor, materialLocation.specularShininess, materialLocation.specularIntensity,
                     materialLocation.reflectivity, materialLocation.translucency);
        
        for(size_t i = 0; i < object->meshes.size(); i++) {
            auto& texture = object->albedos[object->meshToTex[i]];
            if(object->normals.size() > 0) {
                Shader::seti1(useNormalMapLocation, 1);
                auto& normalMap = object->normals[object->meshToTex[i]];
                normalMap.use(GL_TEXTURE3 + maxPointLights + maxSpotLights);    
            } else {
                Shader::seti1(useNormalMapLocation, 0);
            }
            if(object->displacements.size() > 0) {
                Shader::seti1(useDisplaceMapLocation, 1);
                auto& displaceMap = object->displacements[object->meshToTex[i]];
                displaceMap.use(GL_TEXTURE4 + maxPointLights + maxSpotLights);  
            } else {
                Shader::seti1(useDisplaceMapLocation, 0);
            }
            texture.use(GL_TEXTURE0);
            auto& mesh = object->meshes[i];
            mesh.render();
        }
    }

    // guide.render(view, projection);
}
