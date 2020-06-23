#version 450

// Inputs
layout (location=0) in vec3 pos;
layout (location=1) in vec2 tex;
layout (location=2) in vec3 nrm;
layout (location=3) in vec3 tgt;

// Outputs
out vec4 vCol;
out vec2 vTex;
out vec3 fNrm;
out vec3 fPos;
out vec4 vPos;
out mat3 TBN;
out vec4 directionalLightSpacePos;

// Uniforms
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 directionalLightTransform;
uniform vec2 uvScale;

void main() {
    vPos = view * model * vec4(pos, 1.0);
    directionalLightSpacePos = directionalLightTransform * model * vec4(pos, 1.f);

    vCol = vec4(clamp(pos, 0.f, 1.f), 1.f);
    vTex = tex * uvScale;
    fNrm = normalize(mat3(transpose(inverse(model))) * nrm);
    fPos = (model * vec4(pos, 1.f)).xyz;

    vec3 T = normalize(mat3(transpose(inverse(model))) * tgt);
    vec3 N = normalize(mat3(transpose(inverse(model))) * nrm); //normalize(vec3(model * vec4(nrm, 0.0)));
    T = normalize(T - dot(T, N) * N);
    vec3 B = normalize(cross(N, T));
    TBN = mat3(T, B, N);

    gl_Position = projection * view * model * vec4(pos, 1.0);
}
