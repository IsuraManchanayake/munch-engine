#include "Core/Color.h"
#include "Core/Math.h"

namespace Color {

const glm::vec3 white      = glm::vec3{ 100, 100, 100 } / 100.0f;
const glm::vec3 lightgray  = glm::vec3{  75,  75,  75 } / 100.0f;
const glm::vec3 darkgray   = glm::vec3{  50,  50,  50 } / 100.0f;
const glm::vec3 black      = glm::vec3{   0,   0,   0 } / 100.0f;
const glm::vec3 red        = glm::vec3{ 100,   0,   0 } / 100.0f;
const glm::vec3 lowred     = glm::vec3{  50,   0,   0 } / 100.0f;
const glm::vec3 yellow     = glm::vec3{ 100, 100,   0 } / 100.0f;
const glm::vec3 lowyellow  = glm::vec3{  50,  50,   0 } / 100.0f;
const glm::vec3 green      = glm::vec3{   0, 100,   0 } / 100.0f;
const glm::vec3 lowgreen   = glm::vec3{   0,  50,   0 } / 100.0f;
const glm::vec3 cyan       = glm::vec3{   0, 100, 100 } / 100.0f;
const glm::vec3 lowcyan    = glm::vec3{   0,  50,  50 } / 100.0f;
const glm::vec3 blue       = glm::vec3{   0,   0, 100 } / 100.0f;
const glm::vec3 lowblue    = glm::vec3{   0,   0,  50 } / 100.0f;
const glm::vec3 magenta    = glm::vec3{ 100,   0, 100 } / 100.0f;
const glm::vec3 lowmagenta = glm::vec3{  50,   0,  50 } / 100.0f;

glm::vec3 hsv(float h, float s, float v) {
    h = std::fmod(h, 360);
    s = clamp(s / 100.f, 0.0f, 1.0f);
    v = clamp(v / 100.f, 0.0f, 1.0f);
    float c = v * s;
    float h_ = h / 60;
    float x = c * (1 - std::abs(std::fmod(h_, 2) - 1));
    glm::vec3 rgb1 { 0.f, 0.f, 0.f };
    if(0 <= h_ && h_ <= 1) {
        rgb1 = {   c,   x, 0.f };
    } else if(1 <  h_ && h_ <= 2) {
        rgb1 = {   x,   c, 0.f };
    } else if(2 <  h_ && h_ <= 3) {
        rgb1 = { 0.f,   c,   x };
    } else if(3 <  h_ && h_ <= 4) {
        rgb1 = { 0.f,   x,   c };
    } else if(4 <  h_ && h_ <= 5) {
        rgb1 = {   x, 0.f,   c };
    } else if(5 <  h_ && h_ <= 6) {
        rgb1 = {   c, 0.f,   x };
    }
    return rgb1 + (v - c) * glm::vec3{ 1.f, 1.f, 1.f};
}

}