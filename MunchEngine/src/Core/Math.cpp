#include "Core/Math.h"

#include <random>

float randf(float a, float b) {
    static std::default_random_engine e(6969);
    static std::uniform_real_distribution<float> dis(a, b);
    return dis(e);
}

glm::vec3 randvec3() {
    return {randf(), randf(), randf()};
}

glm::vec3 randunitvec3() {
    return glm::normalize(randvec3());
}

float mapf(float x, float a, float b, float p, float q) {
    return p + (q - p) * ((x - a) / (x - b));
}