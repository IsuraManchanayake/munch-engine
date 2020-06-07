#include "Graphics/Camera.h"
#include "Physics/Physics.h"

Camera::Camera(glm::vec3 position, glm::vec3 worldUp, GLfloat yaw, 
    GLfloat pitch, GLfloat speed, GLfloat turnSpeed)
    : position(position), front(0.f, 0.f, -1.f), up(), right(), worldUp(worldUp),
    yaw(yaw), pitch(pitch), speed(speed), turnSpeed(turnSpeed) {
    update();
}

Camera::~Camera() {
}

void Camera::update() {
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y =                      1 * sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(front);
    right = glm::normalize(glm::cross(front, worldUp));
    up    = glm::normalize(glm::cross(right, front));
}

void Camera::keyControl(const std::array<uint16_t, 1024>& keys) {
    float dt = Physics::dt;
    if(keys[GLFW_KEY_SPACE]) {
        dt *= 6.f;
    }
    if (keys[GLFW_KEY_W]) {
        position += front * speed * dt;
    }
    if (keys[GLFW_KEY_S]) {
        position -= front * speed * dt;
    }
    if (keys[GLFW_KEY_A]) {
        position -= right * speed * dt;
    }
    if (keys[GLFW_KEY_D]) {
        position += right * speed * dt;
    }
    if(keys[GLFW_KEY_Q]) {
        position += up * speed * dt;
    }
    if(keys[GLFW_KEY_E]) {
        position -= up * speed * dt;
    }
    if(keys[GLFW_KEY_LEFT_SHIFT]) {
        position += worldUp * speed * dt;
    }
    if(keys[GLFW_KEY_LEFT_CONTROL]) {
        position -= worldUp * speed * dt;
    }
}

void Camera::mouseControl(GLfloat dx, GLfloat dy) {
    float dtmod = 10.f * Physics::dt;
    yaw += dx * turnSpeed * dtmod;
    pitch -= dy * turnSpeed * dtmod;

    if (pitch > 89.f) {
        pitch = 89.f;
    }
    if (pitch < -89.f) {
        pitch = -89.f;
    }

    update();
}

glm::mat4 Camera::viewMat() const {
    return glm::lookAt(position, position + front, up);
}