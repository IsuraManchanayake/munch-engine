#version 450

layout(vertices=3) out;

uniform vec3 eyePos;

in vec2 cs_in_vTex[];
in vec3 cs_in_fPos[];
in vec3 cs_in_T[];
in vec3 cs_in_N[];
in vec4 cs_in_directionalLightSpacePos[];

out vec2 es_in_vTex[];
out vec3 es_in_fPos[];
out vec3 es_in_T[];
out vec3 es_in_N[];
out vec4 es_in_directionalLightSpacePos[];

float getTessLevel(float Distance0, float Distance1) {
    float avgDistance = (Distance0 + Distance1) / 2.0;
    return max(0.0, 1000 / pow(avgDistance, 1.8) + 0.0);
}

void main() {
    es_in_vTex[gl_InvocationID] = cs_in_vTex[gl_InvocationID];
    es_in_fPos[gl_InvocationID] = cs_in_fPos[gl_InvocationID];
    es_in_T[gl_InvocationID] = cs_in_T[gl_InvocationID];
    es_in_N[gl_InvocationID] = cs_in_N[gl_InvocationID];
    es_in_directionalLightSpacePos[gl_InvocationID] = cs_in_directionalLightSpacePos[gl_InvocationID];

    float dist0 = distance(eyePos, es_in_fPos[0]);
    float dist1 = distance(eyePos, es_in_fPos[1]);
    float dist2 = distance(eyePos, es_in_fPos[2]);

    if(gl_InvocationID == 0) {
        gl_TessLevelOuter[0] = mix(1, gl_MaxTessGenLevel / 10, getTessLevel(dist1, dist2));
        gl_TessLevelOuter[1] = mix(1, gl_MaxTessGenLevel / 10, getTessLevel(dist0, dist2));
        gl_TessLevelOuter[2] = mix(1, gl_MaxTessGenLevel / 10, getTessLevel(dist0, dist1));
        gl_TessLevelInner[0] = gl_TessLevelOuter[2];
    }
}
