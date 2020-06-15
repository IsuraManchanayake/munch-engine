
#version 330

layout (location=0) out vec3 color;

in vec2 vTex;
in vec3 vPos;

uniform sampler2D hdri;
uniform float delta;

#define PI 3.1415926535897932384626433832795

vec3 get(vec3 p) {
    p = normalize(p);
    float y = acos(p.y) / PI;
    float angle = atan(p.x / p.z);
    if(p.z < 0) {
        angle = PI + angle;
    }
    float x = angle / (2 * PI);
    vec3 sample = texture(hdri, vec2(x, y)).rgb;
    return sample;
}

void main() {
    float phi_ = vTex.x * 2 * PI;
    float theta_ = (vTex.y) * PI;
    vec3 pos = vec3(sin(theta_) * sin(phi_) , cos(theta_), sin(theta_) * cos(phi_)); 

    // vec3 pos = vTex;
    vec3 normal = normalize(pos);
    vec3 irradiance = vec3(0.0);

    vec3 up = vec3(0.f, 1.f, 0.f);
    vec3 right = cross(up, normal);
    up = cross(normal, right);

    int samples = 0;
    for(float phi = 0.f; phi < PI * 2; phi += delta) {
        for(float theta = 0.f; theta < PI / 2; theta += delta) {
            vec3 tangent = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            vec3 sampleP = tangent.x * right + tangent.y * up + tangent.z * normal;
            irradiance += get(sampleP) * cos(theta) * sin(theta);
            samples++;
        }
    }
    irradiance = (PI * irradiance) / (samples);

    color = irradiance;
    color.rgb = log2(color.rgb + 1.0);
    // color.rgb = color.rgb / (color.rgb + 1.0);
    color.rgb = pow(color.rgb, vec3(1/2.2));
}