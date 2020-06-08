#include "Core/Math.h"
#include "Core/Common.h"

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

float lerpf(float a, float b, float x) {
    return a + (b - a) * x;
}

namespace PerlinImpl {

using vec2 = std::pair<float, float>;

float dot(const vec2& a, const vec2& b) {
    return a.first * b.first + a.second * b.second;
}

float fade(float t) {
    return t * t * t * (3 * t * (2 * t - 5) + 10);
}

float perlin2D(float x, float y) {
    static const unsigned gh = 255;
    static const unsigned gw = 255;

    static const vec2 randv[] = {
        { -1.f, -1.f }, { -1.f,  1.f },
        {  1.f, -1.f }, {  1.f,  1.f }
    };

    static vec2 field[gh][gw];
    static bool initField = false;
    if(!initField) {
        for(size_t i = 0; i < gh; i++) {
            for(size_t j = 0; j < gw; j++) {
                size_t r = static_cast<size_t>(randf(0.f, ::size(randv)));
                field[i][j] = randv[r];
            }
        }
        initField = true;
    }
    
    int x0 = static_cast<int>(floor(x));
    int y0 = static_cast<int>(floor(y));
    int x1 = static_cast<int>(x0 + 1);
    int y1 = static_cast<int>(y0 + 1);

    float dx = x - x0;
    float dy = y - y0;

    vec2& v00 = field[y0 % gh][x0 % gw];
    vec2& v01 = field[y1 % gh][x0 % gw];
    vec2& v10 = field[y0 % gh][x1 % gw];
    vec2& v11 = field[y1 % gh][x1 % gw];

    vec2  w00 = { dx    , dy     };
    vec2  w01 = { dx    , dy - 1 };
    vec2  w10 = { dx - 1, dy     };
    vec2  w11 = { dx - 1, dy - 1 };

    float d00 = dot(w00, v00);
    float d01 = dot(w01, v01);
    float d10 = dot(w10, v10);
    float d11 = dot(w11, v11);

    float dx0 = lerpf(d00, d10, fade(dx));
    float dx1 = lerpf(d01, d11, fade(dx));
    float df  = lerpf(dx0, dx1, fade(dy));

    return mapf(df, -1.f, 1.f, 0.f, 1.f);
}
}

float perlin2D(float x, float y) {
    return PerlinImpl::perlin2D(x, y);
}

float mapf(float x, float a, float b, float p, float q) {
    return p + (q - p) * ((x - a) / (x - b));
}