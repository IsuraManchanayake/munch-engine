#version 450

layout (location=0) in vec3 pos;

uniform mat4 modelM;
uniform mat4 projectionM;
uniform mat4 viewM;

void main() {
    gl_Position = projectionM * viewM * modelM * vec4(pos, 1.0f);
}
