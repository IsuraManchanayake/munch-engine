#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>

struct Camera {
    Camera(glm::vec3 position, glm::vec3 worldUp, GLfloat yaw,
        GLfloat pitch, GLfloat speed, GLfloat turnSpeed);
    ~Camera();

    void update();
    void keyControl(const std::array<uint16_t, 1024>& keys);
    void mouseControl(GLfloat dx, GLfloat dy);
    glm::mat4 viewMat() const;

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    GLfloat yaw; // left - right
    GLfloat pitch; // up - down

    GLfloat speed;
    GLfloat turnSpeed;
};

