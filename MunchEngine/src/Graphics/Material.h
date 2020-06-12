#pragma once

#include "Graphics/Texture.h"

#include <string>
#include <glm/glm.hpp>

enum class TextureMapType {
    albedo = 0, // Keep at 0
    normal = 1,
    displacement = 2,
    specular = 3,
    gloss = 4,
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
    void attachColor(TextureMapType type, const glm::vec3& color);
    void attachColor(TextureMapType type, float r, float g, float b);
    void attachDefault(TextureMapType type);

    static Material defaultMaterial();
    
    union {
        struct {
            Texture albedo;
            Texture normal;
            Texture displacement;
            Texture specular;
            Texture gloss;
        };
        Texture maps[Material::textureTypeCount];
    };
};