#version 450

// Inputs
layout (location=0) in vec3 pos;
layout (location=1) in vec2 tex;
layout (location=2) in vec3 nrm;
layout (location=3) in vec3 tgt;

// Outputs
out vec2 cs_in_vTex;
out vec3 cs_in_fPos;
out vec3 cs_in_T;
out vec3 cs_in_N;
out vec4 cs_in_directionalLightSpacePos;

// Uniforms
uniform mat4 model;
uniform mat4 directionalLightTransform;
uniform vec2 uvScale;

void main() {
    vec4 worldPos = model * vec4(pos, 1.f);
    cs_in_directionalLightSpacePos = directionalLightTransform * worldPos;

    cs_in_vTex = tex * uvScale;
    cs_in_fPos = worldPos.xyz;

    cs_in_T = normalize(mat3(transpose(inverse(model))) * tgt);
    cs_in_N = normalize(mat3(transpose(inverse(model))) * nrm);
    cs_in_T = normalize(cs_in_T - dot(cs_in_T, cs_in_N) * cs_in_N);
    // vec3 B = normalize(cross(N, T));
    // TBN = mat3(T, B, N);

    // gl_Position = worldPos;
    // gl_Position = projection * view * worldPos;
}
