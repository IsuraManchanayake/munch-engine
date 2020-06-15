#version 330

// Constants
const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;
#define pi 3.14159265358979323846

// Definitions
struct Material {
    sampler2D albedo;
    sampler2D normal;
    sampler2D displacement;
    sampler2D specular;
    sampler2D gloss;
    sampler2D roughness;
    sampler2D metallic;
    sampler2D ao;
};

struct DirectionalLight {
    vec3 color;
    float ambientIntensity;
    vec3 diffuseDirection;
    float diffuseIntensity;
    sampler2D shadowMap;
};

struct PointLight {
    vec3 color;
    vec3 position;
    vec3 constants;
    float intensity;
    samplerCube shadowMap;
    float shadowFar;
};

struct SpotLight {
    vec3 color;
    vec3 position;
    vec3 direction;
    vec3 constants;
    float angle;
    float intensity;
    samplerCube shadowMap;
    float shadowFar;
};

// Outputs
out vec4 color;

// Inputs
in vec2 vTex;
// in vec3 fNrm;
in vec3 fPos;
in vec4 vPos;
in mat3 TBN;
in vec4 directionalLightSpacePos;

// Global variables
vec3 fAlbedo;
vec3 fNrm;
float fSpecular;
float fGloss;
float fRough;
float fMetal;
float fAO;
vec3 fIrradiance;

// uniforms
uniform DirectionalLight directionalLight;
uniform Material material;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_POINT_LIGHTS];
uniform int pointLightCount;
uniform int spotLightCount;
uniform vec3 eye;
uniform sampler2D irradianceMap;

vec2 xyz_to_uv(vec3 p) {
    p = normalize(p);
    float y = acos(p.y) / pi;
    float angle = atan(p.x / p.z);
    if(p.z < 0) {
        angle = pi + angle;
    }
    float x = angle / (2 * pi);
    return vec2(x, y);
}

float map(float x, float a, float b, float p, float q) {
    return p + (q - p) * ((x - a) / (b - a));
}

float getDirectionalShadowFactor(DirectionalLight light) {
    float shadow = 0.f;
    vec3 projCoords = directionalLightSpacePos.xyz / directionalLightSpacePos.w;
    projCoords = projCoords * 0.5 + 0.5;
    float current = projCoords.z;
    vec3 normal = normalize(fNrm);
    vec3 lightDir = normalize(-light.diffuseDirection);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    vec2 texelSize = 1.0 / textureSize(light.shadowMap, 0);
    for(int y = -1; y <= 1; ++y) {
        for(int x = -1; x <= 1; ++x) {
            float pcfDepth = texture(light.shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += current - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

vec3 gridSamplingDisk[20] = vec3[] (
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float getOmniShadowFactorP(PointLight light) {
    vec3 fragToLight = fPos - light.position;
    float currentDepth = length(fragToLight);
    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(eye - fPos);
    float far_plane = light.shadowFar;
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(light.shadowMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far_plane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);  
        
    return shadow;
}

float getOmniShadowFactorS(SpotLight light) {
    vec3 fragToLight = fPos - light.position;
    float currentDepth = length(fragToLight);
    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(eye - fPos);
    float far_plane = light.shadowFar;
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(light.shadowMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far_plane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);  
        
    return shadow;
}


vec4 diffuse(vec3 color, vec3 direction, float intensity, float shadowFactor) {
    vec3 viewDir = normalize(eye - fPos);
    vec3 half = normalize(viewDir + direction);

    float alpha = fRough * fRough;
    float alpha2 = alpha * alpha;
    float ndf_ggxtr = alpha2 / (pi * pow(pow(max(0, dot(half, fNrm)), 2) * (alpha2 - 1) + 1, 2));
    
    float k_geometry = (fRough + 1) * (fRough + 1) / 8;
    float ndotv = max(0, dot(fNrm, viewDir));
    float ndotl = max(0, dot(fNrm, direction));
    float g_obs = ndotv / (ndotv * (1 - k_geometry) + k_geometry);
    float g_shd = ndotl / (ndotl * (1 - k_geometry) + k_geometry);
    float g_schlickggx = g_obs * g_shd;
    
    vec3 f0 = vec3(0.04);
    f0 = mix(f0, fAlbedo, fMetal);
    vec3 f_schlick = f0 + (1 - f0) * pow(1 - max(0, dot(half, fNrm)), 5);

    vec3 f_cooktorrance = (ndf_ggxtr * f_schlick * g_schlickggx) / (4 * max(0.001, dot(viewDir, fNrm)) * max(0.001, dot(direction, fNrm)));
    
    vec3 k_s = f_schlick;
    vec3 k_d = 1 - k_s; // ??
    k_d *= 1 - fMetal;
    vec3 f_lambert = fAlbedo / pi;

    vec3 f_r = k_d * f_lambert + f_cooktorrance;
    float factor = max(0, dot(fNrm, direction));
    float attenuation = 1.0f; // calculate 
    vec3 radiance = color * attenuation * intensity;
    vec3 L0 = f_cooktorrance * radiance * factor;
    return vec4(L0 * (1 - shadowFactor), 1.0);
    // return vec4(fAlbedo, 1.0);
}

vec4 getAmbientColor() {
    vec3 viewDir = normalize(eye - fPos);
    vec3 f0 = vec3(0.04);
    f0 = mix(f0, fAlbedo, fMetal);
    vec3 k_s = f0 + (max(vec3(1.0 - fRough), f0) - f0) * pow(1 - max(0, dot(viewDir, fNrm)), 5);
    // vec3 k_s = f0 + (max(vec3(1.0 - fRough), f0) - f0) * pow(- max(0, dot(viewDir, fNrm)), 5);
    vec3 k_d = 1 - k_s;
    k_d *= 1 - fMetal;
    vec3 diffuse_ = fIrradiance * fAlbedo;
    vec3 ambient = diffuse_ * k_d * fAO;
    return vec4(ambient, 1.0);
}

vec4 getDirectionalColor() {
    float shadowFactor = getDirectionalShadowFactor(directionalLight);
    return diffuse(pow(directionalLight.color, vec3(2.2)), -directionalLight.diffuseDirection, directionalLight.diffuseIntensity, shadowFactor);
}

vec4 getPointLightsColor() {
    vec4 result = vec4(0.f, 0.f, 0.f, 0.f);
    for(int i = 0; i < pointLightCount; i++) {
        if(pointLights[i].intensity > 1e-5) {
            vec3 distVec = pointLights[i].position - fPos;
            float dist = sqrt(dot(distVec, distVec));
            float intensity = pointLights[i].intensity 
                                / (pointLights[i].constants.x * dist * dist + pointLights[i].constants.y * dist + pointLights[i].constants.z);
            float shadowFactor = getOmniShadowFactorP(pointLights[i]);
            result += diffuse(pow(pointLights[i].color, vec3(2.2)), distVec, intensity, shadowFactor);
        }
    }
    return result;
}

vec4 getSpotLightsColor() {
    vec4 result = vec4(0.f, 0.f, 0.f, 0.f);
    for(int i = 0; i < spotLightCount; i++) {
        if(spotLights[i].intensity > 1e-5) {
            vec3 distVec = fPos - spotLights[i].position;
            float dist = sqrt(dot(distVec, distVec));
            float angle = acos(dot(distVec / dist, normalize(spotLights[i].direction)));
            if(angle < spotLights[i].angle) {
                float intensity = spotLights[i].intensity 
                                    / (spotLights[i].constants.x * dist * dist + spotLights[i].constants.y * dist + spotLights[i].constants.z);
                float fadedIntensity = map(angle, 0.f, spotLights[i].angle, 1.f, 0.f) * intensity;
                float shadowFactor = getOmniShadowFactorS(spotLights[i]);
                result += diffuse(spotLights[i].color, -distVec, fadedIntensity, shadowFactor);
            }
        }
    }
    return result;
}

void main() {
    fNrm = texture(material.normal, vTex).rgb;
    fNrm = fNrm * 2.0 - 1.0;   
    fNrm = normalize(TBN * fNrm);

    fAlbedo = pow(texture(material.albedo, vTex).rgb, vec3(2.2));
    fSpecular = texture(material.specular, vTex).r;
    fGloss = texture(material.gloss, vTex).r * 255.0f;
    fRough = max(0.025, texture(material.roughness, vTex).r);
    fMetal = texture(material.metallic, vTex).r;
    fAO = texture(material.ao, vTex).r;
    fIrradiance = clamp(texture(irradianceMap, xyz_to_uv(fNrm)).rgb, 0.0, 1.0);

    vec4 ambientColor = getAmbientColor();
    vec4 directionalColor = max(vec4(0.0), getDirectionalColor());
    vec4 pointLightsColor = getPointLightsColor();
    vec4 spotLightsColor = getSpotLightsColor();

    // color = vec4(fMetal);
    // color = ambientColor;
    color = directionalColor + pointLightsColor + ambientColor;
    // color = vec4(fAlbedo, 1.0);
    // color = directionalColor + pointLightsColor + ambientColor; // + ambientColor; // + pointLightsColor + spotLightsColor;// + ambientColor;
    color.rgb = color.rgb / (color.rgb + vec3(1.0));
    // color.rgb = log2(color.rgb + vec3(1.0));
    color.rgb = pow(color.rgb, vec3(1.0/2.2));
    color.a = 1.0;
    // color = vec4(vec3(fAO), 1);
    
    // color = texture(material.albedo, vTex) * (ambientColor + directionalColor);
    // color.rgb = pow(color.rgb, vec3(1/2.2));

    // Logify
    // color.rgb = log2(color.rgb + 1);
    // Desaturate 
    // vec3 grayXfer = vec3(0.3, 0.59, 0.11);
	// vec3 gray = vec3(dot(grayXfer, color.rgb));
	// color.rgb = mix(color.rgb, gray, 0.3);
    
    // Contrast
    // color.rgb = (color.rgb - 0.5f) * max(2.0f, 0.0f) + 0.5f;
}
