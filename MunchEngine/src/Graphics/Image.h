#pragma once

#include <glm/glm.hpp>
#include <string>

struct Image {
    Image(const std::string& path);
    ~Image();
    glm::vec4 uv(float u, float v) const;

    std::string path;
    int width;
    int height;
    int bitDepth;
    unsigned char* data;
};