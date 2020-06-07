#version 330

in vec2 vTex;

uniform sampler2D theTexture;

out vec4 color;

void main() {
    color = texture(theTexture, vTex);
}
