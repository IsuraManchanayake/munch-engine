#pragma once

#include <glm/glm.hpp>

namespace Color {

extern glm::vec3 white;
extern glm::vec3 lightgray;
extern glm::vec3 darkgray;
extern glm::vec3 black;
extern glm::vec3 red;
extern glm::vec3 lowred;
extern glm::vec3 yellow;
extern glm::vec3 lowyellow;
extern glm::vec3 green;
extern glm::vec3 lowgreen;
extern glm::vec3 cyan;
extern glm::vec3 lowcyan;
extern glm::vec3 blue;
extern glm::vec3 lowblue;
extern glm::vec3 magenta;
extern glm::vec3 lowmagenta;

glm::vec3 hsv(float h, float s, float v);

}