#include "Graphics/ShaderFactory.h"
#include "Core/Common.h"

Shader* ShaderFactory::defaultShader() {
    static const unsigned maxPointLights = 3;
    static const unsigned maxSpotLights = 3;
    
    Shader* shader = new Shader;
    shader->create()
          ->addShaderFromPath(ShaderType::Vertex, "Shaders/default.vert.glsl")
          ->addShaderFromPath(ShaderType::Fragment, "Shaders/default.frag.glsl")
          ->compile();
    
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
    shader->create()
          ->addShaderFromPath(ShaderType::Vertex, "Shaders/directional_shadow_map.vert.glsl")
          ->addShaderFromPath(ShaderType::Fragment, "Shaders/directional_shadow_map.frag.glsl")
          ->compile();
    return shader;
}

Shader* ShaderFactory::omniDirShadowShader() {
    Shader* shader = new Shader;
    shader->create()
          ->addShaderFromPath(ShaderType::Vertex, "Shaders/omni_shadow_map.vert.glsl")
          ->addShaderFromPath(ShaderType::Geometry, "Shaders/omni_shadow_map.geom.glsl")
          ->addShaderFromPath(ShaderType::Fragment, "Shaders/omni_shadow_map.frag.glsl")
          ->compile();
    return shader;
}

Shader* ShaderFactory::pbrShader() {
    static const unsigned maxPointLights = 3;
    static const unsigned maxSpotLights = 3;
    Shader* shader = new Shader;
    shader->create()
          ->addShaderFromPath(ShaderType::Vertex, "Shaders/pbr.vert.glsl")
          ->addShaderFromPath(ShaderType::Fragment, "Shaders/pbr.frag.glsl")
          ->compile();
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
    shader->create()
          ->addShaderFromPath(ShaderType::Vertex, "Shaders/terrain.vert.glsl")
          ->addShaderFromPath(ShaderType::TessCtrl, "Shaders/terrain.tesc.glsl")
          ->addShaderFromPath(ShaderType::TessEval, "Shaders/terrain.tese.glsl")
          ->addShaderFromPath(ShaderType::Fragment, "Shaders/terrain.frag.glsl")
          ->compile();
    std::vector<std::string> uniformTextureNames = {
        "directionalLight.shadowMap",
        "material.albedo",
        "material.normal",
        "pmaterial.displacement",
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
    shader->create()
          ->addShaderFromPath(ShaderType::Vertex, "Shaders/hdri.vert.glsl")
          ->addShaderFromPath(ShaderType::Fragment, "Shaders/hdri.frag.glsl")
          ->compile();
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
    shader->create()
          ->addShaderFromPath(ShaderType::Vertex, "Shaders/irradiance.vert.glsl")
          ->addShaderFromPath(ShaderType::Fragment, "Shaders/irradiance.frag.glsl")
          ->compile();
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