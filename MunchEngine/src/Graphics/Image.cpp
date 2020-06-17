#include "Graphics/Image.h"
#include "Core/Logger.h"
#include "Core/Common.h"
// #define STB_IMAGE_IMPLEMENTATION
#include "Graphics/stb_image.h"


Image::Image(const std::string& path) 
    : path(path), width(1), height(1), bitDepth(), data(nullptr) {
    data = stbi_load(
        path.c_str(), &width, &height, &bitDepth, 0);
    if (!data) {
        Logger::error("Failed to load height map data from ", path);
        error_exit(1);
    }
}

Image::~Image() {
    if(data) {
        delete[] data;
    }
}

glm::vec4 Image::uv(float u, float v) const {
    u = fmod(u, 1);
    v = fmod(v, 1);
    float x = u * width;
    float y = v * height;
    unsigned x0 = static_cast<unsigned>(floor(x));
    unsigned y0 = static_cast<unsigned>(floor(y));
    unsigned x1 = (x0 + 1) % width;
    unsigned y1 = (y0 + 1) % height;
    float dx = x - x0;
    float dy = y - y0;
    glm::vec4 c00(0.f);
    glm::vec4 c01(0.f);
    glm::vec4 c10(0.f);
    glm::vec4 c11(0.f);
    if(bitDepth >= 1) {
        c00.r = data[bitDepth * (x0 + y0 * width) + 0] / 255.f;
        c01.r = data[bitDepth * (x0 + y1 * width) + 0] / 255.f;
        c10.r = data[bitDepth * (x1 + y0 * width) + 0] / 255.f;
        c11.r = data[bitDepth * (x1 + y1 * width) + 0] / 255.f;
    } 
    if(bitDepth >= 3) {
        c00.g = data[bitDepth * (x0 + y0 * width) + 1] / 255.f;
        c01.g = data[bitDepth * (x0 + y1 * width) + 1] / 255.f;
        c10.g = data[bitDepth * (x1 + y0 * width) + 1] / 255.f;
        c11.g = data[bitDepth * (x1 + y1 * width) + 1] / 255.f;
        c00.b = data[bitDepth * (x0 + y0 * width) + 2] / 255.f;
        c01.b = data[bitDepth * (x0 + y1 * width) + 2] / 255.f;
        c10.b = data[bitDepth * (x1 + y0 * width) + 2] / 255.f;
        c11.b = data[bitDepth * (x1 + y1 * width) + 2] / 255.f;
    } 
    if(bitDepth >= 4) {
        c00.a = data[bitDepth * (x0 + y0 * width) + 3] / 255.f;
        c01.a = data[bitDepth * (x0 + y1 * width) + 3] / 255.f;
        c10.a = data[bitDepth * (x1 + y0 * width) + 3] / 255.f;
        c11.a = data[bitDepth * (x1 + y1 * width) + 3] / 255.f;
    }
    glm::vec4 d0 = c00 * (1 - dx) + c01 * dx;
    glm::vec4 d1 = c10 * (1 - dx) + c11 * dx;
    glm::vec4 d = d0 * (1 - dy) + d1 * dy;
    return d;
}

