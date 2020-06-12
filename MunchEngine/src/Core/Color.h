#pragma once

#include <glm/glm.hpp>

namespace Color {

extern const glm::vec3 white;
extern const glm::vec3 lightgray;
extern const glm::vec3 darkgray;
extern const glm::vec3 black;
extern const glm::vec3 red;
extern const glm::vec3 lowred;
extern const glm::vec3 yellow;
extern const glm::vec3 lowyellow;
extern const glm::vec3 green;
extern const glm::vec3 lowgreen;
extern const glm::vec3 cyan;
extern const glm::vec3 lowcyan;
extern const glm::vec3 blue;
extern const glm::vec3 lowblue;
extern const glm::vec3 magenta;
extern const glm::vec3 lowmagenta;

glm::vec3 hsv(float h, float s, float v);

}