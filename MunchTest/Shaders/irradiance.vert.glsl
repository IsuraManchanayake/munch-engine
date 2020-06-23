#version 450

layout (location=0) in vec3 pos;
layout (location=1) in vec2 tex;

out vec2 vTex;
out vec3 vPos;

uniform mat4 model;

void main() {
    vTex = tex;
    // vPos = vec3(tex, 0.0);
    gl_Position = model * vec4(pos, 1.0);
}