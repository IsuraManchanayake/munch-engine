#pragma once

#include "Graphics/Texture.h"

#include <string>
#include <glm/glm.hpp>

enum class TextureMapType {
    albedo = 0, // Keep at 0
    normal,
    displacement,
    specular,
    gloss,
    roughness,
    metallic,
    ao,
    None // Keep at the end
};

struct Material {
    static constexpr size_t textureTypeCount = static_cast<size_t>(TextureMapType::None);
    static const unsigned defaultTextureMapWidth = 4;
    static const unsigned defaultTextureMapHeight = 4;
    
    Material();
    ~Material();

    Material& operator=(const Material& tm);
    Material(const Material& tm);

    void attach(TextureMapType type, std::string path);
    void attachSpecular(float intensity);
    void attachGloss(float shininess);
    void attachDisplacement(float displacement);
    void attachRoughness(float roughness);
    void attachMetallic(float metallic);
    void attachAO(float ao);
    void attachColor(TextureMapType type, const glm::vec3& color);
    void attachColor(TextureMapType type, float r, float g, float b);
    void attachColor(TextureMapType type, float r);
    void attachDefault(TextureMapType type);

    static Material defaultMaterial();
    
    union {
        struct {
            Texture albedo;
            Texture normal;
            Texture displacement;
            Texture specular;
            Texture gloss;
            Texture roughness;
            Texture metallic;
            Texture ao;
        };
        Texture maps[Material::textureTypeCount];
    };
};