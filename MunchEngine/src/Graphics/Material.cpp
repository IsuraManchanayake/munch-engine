#include "Graphics/Material.h"
#include "Core/Logger.h"
#include "Core/Math.h"

const unsigned Material::defaultTextureMapWidth;
const unsigned Material::defaultTextureMapHeight;

Material::Material()
    : albedo(), normal(), displacement(), specular(), gloss() {
}

Material::~Material() {
    albedo.clear();
    normal.clear();
    displacement.clear();
    specular.clear();
    gloss.clear();
}

Material& Material::operator=(const Material& tm) {
    for(size_t i = 0; i < textureTypeCount; i++) {
        maps[i] = tm.maps[i];
    }
    return *this;
}

Material::Material(const Material& tm)
    : maps() {
    std::copy(std::begin(tm.maps), std::end(tm.maps), std::begin(maps));
}

void Material::attach(TextureMapType type, std::string path) {
    maps[static_cast<size_t>(type)].create(path);
}

void Material::attachSpecular(float intensity) {
    intensity = clamp(intensity, 0.0f, 1.0f);
    attachColor(TextureMapType::specular, intensity, intensity, intensity);
}

void Material::attachGloss(float shininess) {
    shininess = clamp(shininess, 0.0f, 255.0f);
    shininess /= 255.0f;
    attachColor(TextureMapType::gloss, shininess, shininess, shininess);
}

void Material::attachDisplacement(float displacement) {
    displacement = clamp(displacement, -1.0f, 1.0f);
    displacement = (displacement + 1) / 2;
    attachColor(TextureMapType::displacement, displacement, displacement, displacement);
}

void Material::attachColor(TextureMapType type, const glm::vec3& color) {
    attachColor(type, color.r, color.g, color.b);
}

void Material::attachColor(TextureMapType type, float r, float g, float b) {
    maps[static_cast<size_t>(type)] = Texture::createColorTexture(defaultTextureMapWidth, defaultTextureMapHeight, r, g, b);
}

void Material::attachDefault(TextureMapType type) {
    switch(type) {
        case TextureMapType::albedo:
            attachColor(type, 1.0f, 1.0f, 1.0f);
            break;
        case TextureMapType::normal:
            attachColor(type, 0.5f, 0.5f, 1.0f);
            break;
        case TextureMapType::displacement:
            attachColor(type, 0.5f, 0.5f, 0.5f);
            break;
        case TextureMapType::specular:
            attachColor(type, 0.0f, 0.0f, 0.0f);
            break;
        case TextureMapType::gloss:
            attachColor(type, 0.0f, 0.0f, 0.0f);
            break;
        default:
            Logger::error("Invalid or unimplemented TextureMapType");
    }
}

Material Material::defaultMaterial() {
    Material result;
    for(unsigned i = 0; i < Material::textureTypeCount; i++) {
        result.attachDefault(static_cast<TextureMapType>(i));
    }
    return result;
}
