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