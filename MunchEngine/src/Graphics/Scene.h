#pragma once

#include <GL/glew.h>

#include <vector>

#include "Graphics/Lights/DirectionalLight.h"
#include "Graphics/Lights/PointLight.h"
#include "Graphics/Lights/SpotLight.h"
#include "Graphics/Mesh.h"
#include "Graphics/Shader.h"
#include "Graphics/Camera.h"
#include "Graphics/Model.h"
#include "Graphics/SkyBox.h"

struct Scene {
    // Should match main shader's constant
    static constexpr size_t maxPointLights = 3;
    // Should match main shader's constant
    static constexpr size_t maxSpotLights = 3;

    Scene();
    ~Scene();

    // Configure
    void addModel(const std::string& path, const glm::mat4& transformation, const Material& material);
    void addModel(const Model& model);
    void setCamera(const Camera& camera);
    void setSkyBox(const SkyBox& skyBox);
    void setMainShader(const Shader& shader);
    void setProjection(const glm::mat4& projection);
    void setDirectionalLight(const DirectionalLight& directionalLight);
    void addPointLight(const PointLight& pointLight);
    void addSpotLight(const SpotLight& spotLight);
    void setAllUniforms();

    // Render
    void setTextureUnits();
    void render(Shader& shader);
    void render();

    std::vector<Model> models;
    Camera camera;
    SkyBox skyBox;
    Shader mainShader;
    glm::mat4 projection;
    DirectionalLight directionalLight;
    std::vector<PointLight> pointLights;
    std::vector<SpotLight> spotLights;
    size_t reservedTextureUnits;
    bool mainShaderInited;
    Shader dirShadowShader;
    Shader omniShadowShader;

    // Uniform locations
    struct DirectionalLightLocation {
        GLint ambientColor;
        GLint ambientIntensity;
        GLint diffuseColor;
        GLint diffuseDirection;
        GLint diffuseIntensity;
        GLint shadowMap;
    };

    struct PointLightLocation {
        GLint color;
        GLint position;
        GLint constants;
        GLint intensity;
        GLint shadowMap;
        GLint shadowFar;
    };

    struct SpotLightLocation {
        GLint color;
        GLint position;
        GLint direction;
        GLint constants;
        GLint angle;
        GLint intensity;
        GLint shadowMap;
        GLint shadowFar;
    };

    struct MaterialLocation {
        GLint specularColor;
        GLint specularShininess;
        GLint specularIntensity;
        GLint reflectivity;
        GLint translucency;
    };

    void setCommonUniforms();
    void setPointSpotLightUniforms();

    DirectionalLightLocation directionalLightLocation;
    std::vector<PointLightLocation> pointLightLocations;
    std::vector<SpotLightLocation> spotLightLocations;
    MaterialLocation materialLocation;
    GLint modelLocation;
    GLint projectionLocation;
    GLint viewLocation;
    GLint directionalLightTransformLocation;
    GLint textureLocation;
    GLint pointLightCountLocation;
    GLint spotLightCountLocation;
    GLint eyeLocation;
    GLint skyBoxLocation;
};