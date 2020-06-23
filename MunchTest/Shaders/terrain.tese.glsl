#version 450

layout(triangles, fractional_odd_spacing, cw) in;

struct PhysicalMaterial {
    sampler2D displacement;
};

uniform mat4 projection;
uniform mat4 view;
uniform PhysicalMaterial pmaterial;

in vec2 es_in_vTex[];
in vec3 es_in_fPos[];
in vec3 es_in_T[];
in vec3 es_in_N[];
in vec4 es_in_directionalLightSpacePos[];

out vec2 fs_in_vTex;
out vec3 fs_in_fPos;
out mat3 fs_in_TBN;
out vec4 fs_in_directionalLightSpacePos;

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2) {
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2) {
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

vec4 interpolate4D(vec4 v0, vec4 v1, vec4 v2) {
    return vec4(gl_TessCoord.x) * v0 + vec4(gl_TessCoord.y) * v1 + vec4(gl_TessCoord.z) * v2;
}

void main() {
    fs_in_vTex = interpolate2D(es_in_vTex[0], es_in_vTex[1], es_in_vTex[2]);
    // vec3 es_in_fPos_0 = es_in_fPos[0] + es_in_N[0] * 10 * texture(pmaterial.displacement, es_in_vTex[0]).x;
    // vec3 es_in_fPos_1 = es_in_fPos[1] + es_in_N[1] * 10 * texture(pmaterial.displacement, es_in_vTex[1]).x;
    // vec3 es_in_fPos_2 = es_in_fPos[2] + es_in_N[2] * 10 * texture(pmaterial.displacement, es_in_vTex[2]).x;
    // fs_in_fPos = interpolate3D(es_in_fPos_0, es_in_fPos_1, es_in_fPos_2);
    fs_in_fPos = interpolate3D(es_in_fPos[0], es_in_fPos[1], es_in_fPos[2]);
    vec3 T = normalize(interpolate3D(es_in_T[0], es_in_T[1], es_in_T[2]));
    vec3 N = normalize(interpolate3D(es_in_N[0], es_in_N[1], es_in_N[2]));
    T = normalize(T - dot(T, N) * N);
    vec3 B = normalize(cross(N, T));
    fs_in_TBN = mat3(T, B, N);
    fs_in_directionalLightSpacePos = interpolate4D(es_in_directionalLightSpacePos[0],
                                                   es_in_directionalLightSpacePos[1], 
                                                   es_in_directionalLightSpacePos[2]);

    fs_in_fPos += N * 10 * texture(pmaterial.displacement, fs_in_vTex).x;
    gl_Position = projection * view * vec4(fs_in_fPos, 1.0);
}
