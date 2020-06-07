#version 330

in vec4 fPos;

uniform vec3 lightPos;
uniform float far;

void main() {
    float distance = length(fPos.xyz - lightPos);
    distance /= far;
    gl_FragDepth = distance;
}