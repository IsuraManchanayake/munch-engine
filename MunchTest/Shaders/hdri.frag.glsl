#version 330

in vec3 vTex;
out vec4 color;

uniform sampler2D hdri;

#define PI 3.1415926535897932384626433832795

void main() {
    vec3 p = normalize(vTex);
    float y = acos(p.y) / PI;
    float angle = atan(p.x / p.z);
    if(p.z < 0) {
        angle = PI + angle;
    }
    float x = angle / (2 * PI);
    color = texture(hdri, vec2(x, y));
    color.rgb = log2(color.rgb + 1.0);
    // color.rgb = color.rgb / (color.rgb + 1.0);
    color.rgb = pow(color.rgb, vec3(1/2.2));
    color.a = 1.0;
}
