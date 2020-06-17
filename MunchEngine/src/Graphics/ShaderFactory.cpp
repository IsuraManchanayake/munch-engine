#include "Graphics/ShaderFactory.h"
#include "Core/Common.h"

Shader* ShaderFactory::defaultShader() {
    static const unsigned maxPointLights = 3;
    static const unsigned maxSpotLights = 3;
    
    Shader* shader = new Shader;
    shader->create("Shaders/default.vert.glsl", "Shaders/default.frag.glsl");
    
    std::vector<std::string> uniformTextureNames = {
        "directionalLight.shadowMap",
        "material.albedo",
        "material.normal",
        "material.displacement",
        "material.specular",
        "material.gloss",
        "hdri",
    };
    for(unsigned i = 0; i < maxPointLights; i++) {
        uniformTextureNames.push_back(cat("pointLights[", i, "].shadowMap"));
    }
    for(unsigned i = 0; i < maxSpotLights; i++) {
        uniformTextureNames.push_back(cat("spotLights[", i, "].shadowMap"));
    }
    for(size_t i = 0; i < uniformTextureNames.size(); i++) {
        shader->tunits[uniformTextureNames[i]] = i;
    }

    // Bind texture units
    shader->use();
    for(auto& [uniformName, tunit] : shader->tunits) {
        shader->seti1(uniformName, tunit);
    }

    return shader;
}

Shader* ShaderFactory::dirShadowShader() {
    Shader* shader = new Shader;
    shader->create("Shaders/directional_shadow_map.vert.glsl", "Shaders/directional_shadow_map.frag.glsl");
    return shader;
}

Shader* ShaderFactory::omniDirShadowShader() {
    Shader* shader = new Shader;
    shader->create("Shaders/omni_shadow_map.vert.glsl", "Shaders/omni_shadow_map.geom.glsl", "Shaders/omni_shadow_map.frag.glsl");
    return shader;
}

Shader* ShaderFactory::pbrShader() {
    static const unsigned maxPointLights = 3;
    static const unsigned maxSpotLights = 3;
    Shader* shader = new Shader;
    shader->create("Shaders/pbr.vert.glsl", "Shaders/pbr.frag.glsl");
    std::vector<std::string> uniformTextureNames = {
        "directionalLight.shadowMap",
        "material.albedo",
        "material.normal",
        "material.displacement",
        "material.specular",
        "material.gloss",
        "material.roughness",
        "material.metallic",
        "material.ao",
        "irradianceMap",
    };
    for(unsigned i = 0; i < maxPointLights; i++) {
        uniformTextureNames.push_back(cat("pointLights[", i, "].shadowMap"));
    }
    for(unsigned i = 0; i < maxSpotLights; i++) {
        uniformTextureNames.push_back(cat("spotLights[", i, "].shadowMap"));
    }
    for(size_t i = 0; i < uniformTextureNames.size(); i++) {
        shader->tunits[uniformTextureNames[i]] = i;
    }

    // Bind texture units
    shader->use();
    for(auto& [uniformName, tunit] : shader->tunits) {
        shader->seti1(uniformName, tunit);
    }
    return shader;
}

Shader* ShaderFactory::terrainShader() {
    static const unsigned maxPointLights = 3;
    static const unsigned maxSpotLights = 3;
    Shader* shader = new Shader;
    shader->create("Shaders/terrain.vert.glsl", "Shaders/terrain.frag.glsl");
    std::vector<std::string> uniformTextureNames = {
        "directionalLight.shadowMap",
        "material.albedo",
        "material.normal",
        "material.displacement",
        "material.specular",
        "material.gloss",
        "material.roughness",
        "material.metallic",
        "material.ao",
        "irradianceMap",
        // "heightMap",
    };
    for(unsigned i = 0; i < maxPointLights; i++) {
        uniformTextureNames.push_back(cat("pointLights[", i, "].shadowMap"));
    }
    for(unsigned i = 0; i < maxSpotLights; i++) {
        uniformTextureNames.push_back(cat("spotLights[", i, "].shadowMap"));
    }
    for(size_t i = 0; i < uniformTextureNames.size(); i++) {
        shader->tunits[uniformTextureNames[i]] = i;
    }

    // Bind texture units
    shader->use();
    for(auto& [uniformName, tunit] : shader->tunits) {
        shader->seti1(uniformName, tunit);
    }
    return shader;
}

Shader* ShaderFactory::hdriShader() {
    Shader* shader = new Shader;
    shader->create("Shaders/hdri.vert.glsl", "Shaders/hdri.frag.glsl");
    std::vector<std::string> uniformTextureNames = {
        "hdri",
    };
    for(size_t i = 0; i < uniformTextureNames.size(); i++) {
        shader->tunits[uniformTextureNames[i]] = i;
    }
    shader->use();
    for(auto& [uniformName, tunit] : shader->tunits) {
        shader->seti1(uniformName, tunit);
    }
    return shader;
}

Shader* ShaderFactory::irradianceShader() {
    Shader* shader = new Shader;
    shader->create("Shaders/irradiance.vert.glsl", "Shaders/irradiance.frag.glsl");
    std::vector<std::string> uniformTextureNames = {
        "hdri",
    };
    for(size_t i = 0; i < uniformTextureNames.size(); i++) {
        shader->tunits[uniformTextureNames[i]] = i;
    }
    shader->use();
    for(auto& [uniformName, tunit] : shader->tunits) {
        shader->seti1(uniformName, tunit);
    }
    return shader;
}