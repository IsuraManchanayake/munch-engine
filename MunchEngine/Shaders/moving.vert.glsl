#version 330

layout (location=0) in vec3 pos;
layout (location=1) in vec2 tex;
layout (location=2) in vec3 nrm;

out vec4 vCol;
out vec2 vTex;
out vec3 vNrm;
out vec3 fPos;
out vec4 vPos;
out vec4 directionalLightSpacePos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

uniform mat4 directionalLightTransform;

void main() {
   gl_Position = projection * view * model * vec4(pos, 1.0);
   vPos = view * model * vec4(pos, 1.0);

   directionalLightSpacePos = directionalLightTransform * model * vec4(pos, 1.f);

   vCol = vec4(clamp(pos, 0.f, 1.f), 1.f);
   vTex = tex;
   vNrm = normalize(mat3(transpose(inverse(model))) * nrm);
   fPos = (model * vec4(pos, 1.f)).xyz;
}
