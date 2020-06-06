#pragma once

#include <glm/glm.hpp>
#include <algorithm>

float randf(float a=0.f, float b=1.f);

glm::vec3 randvec3();

glm::vec3 randunitvec3();

float mapf(float x, float a, float b, float p, float q);

template<typename T>
T clamp(const T& x, const T& a, const T& b);

// ================ Template definitions ================

template<typename T>
inline T clamp(const T& x, const T& a, const T& b) {
    return std::max(std::min(x, b), a);
}