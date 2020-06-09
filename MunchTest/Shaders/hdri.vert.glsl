#version 330

layout (location=0) in vec3 pos;

out vec3 vTex;

uniform mat4 view;
uniform mat4 projection;

void main() {
    vTex = pos;
    gl_Position = projection * view * vec4(pos, 1.0);
}