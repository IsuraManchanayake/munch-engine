#version 450

layout (location=0) in vec3 pos;
layout (location=1) in vec2 tex;
layout (location=2) in vec3 nrm;

out vec2 vTex;

uniform mat4 model;

void main() {
    vTex = tex;
    gl_Position = model * vec4(pos, 1.0f);
}