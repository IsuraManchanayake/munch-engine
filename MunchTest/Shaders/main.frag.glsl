#version 330

out vec4 color;

in vec2 vTex;
in vec3 fNrm;
in vec3 fPos;
in vec4 vPos;
in mat3 TBN;
in vec4 directionalLightSpacePos;

vec3 vNrm;

uniform sampler2D theTexture;
uniform sampler2D normalMap;
uniform samplerCube skyBox;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;
uniform int pointLightCount;
uniform int spotLightCount;
uniform int useNormalMap;

struct DirectionalLight {
    vec3 ambientColor;
    float ambientIntensity;
    vec3 diffuseColor;
    vec3 diffuseDirection;
    float diffuseIntensity;
    sampler2D shadowMap;
};

struct Material {
    vec3 specularColor;
    float specularShininess;
    float specularIntensity;
    float reflectivity;
    float translucency;
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

// uniform OmniShadowMap omniShadowMap;

uniform DirectionalLight directionalLight;
uniform Material material;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_POINT_LIGHTS];
uniform vec3 eye;

float map(float x, float a, float b, float p, float q) {
    return p + (q - p) * ((x - a) / (b - a));
}

float getDirectionalShadowFactor(DirectionalLight light) {
    float shadow = 0.f;
    vec3 projCoords = directionalLightSpacePos.xyz / directionalLightSpacePos.w;
    projCoords = projCoords * 0.5 + 0.5;
    float current = projCoords.z;
    vec3 normal = normalize(vNrm);
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

vec3 gridSamplingDisk[20] = vec3[]
(
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
    if(intensity < 1e-5) {
        return vec4(0.f, 0.f, 0.f, 0.f);
    }
    float diffuseFactor = max(0.f, dot(normalize(vNrm), normalize(direction)));
    float specularFactor = 0.f;
    if(diffuseFactor > 0.f) {
        vec3 reflected = normalize(reflect(-direction, vNrm));
        vec3 eyeDirection = normalize(eye - fPos);
        float reflectFactor = dot(reflected, eyeDirection);
        if(reflectFactor > 0.f) {
            specularFactor = pow(reflectFactor, material.specularShininess);
        }
    }
    vec3 result = color * diffuseFactor
                        + material.specularColor * specularFactor * material.specularIntensity;
    return vec4(result, 1.f) * intensity * (1 - shadowFactor);
}

vec4 getAmbientColor() {
    return vec4(directionalLight.ambientColor, 1.f) * directionalLight.ambientIntensity;
}

vec4 getDirectionalColor() {
    float shadowFactor = getDirectionalShadowFactor(directionalLight);
    return diffuse(directionalLight.diffuseColor, -directionalLight.diffuseDirection, directionalLight.diffuseIntensity, shadowFactor);
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
            result += diffuse(pointLights[i].color, distVec, intensity, shadowFactor);
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

vec4 getSkyBoxReflectColor() {
    vec4 skyBoxColorReflected = vec4(0.f, 0.f, 0.f, 1.f);
    if(material.reflectivity > 1e-4f) {
        vec3 look = fPos - eye;
        vec3 reflected = normalize(reflect(look, vNrm));
        skyBoxColorReflected = texture(skyBox, reflected);
    }
    return skyBoxColorReflected  * material.reflectivity;
}

vec4 getSkyBoxRefractColor() {
    vec4 skyBoxColorRefracted = vec4(0.f, 0.f, 0.f, 1.f);
    if(material.translucency > 1e-4f) {
        vec3 u = fPos - eye;
        vec3 n = normalize(vNrm);
        vec3 m = cross(n, cross(n, u));
        float a = acos(dot(normalize(-u), n));
        float sined = sin(a) / 2.40f;
        if(sined < 1) { 
            float b = asin(sined);
            vec3 refracted = -n * cos(b) - m * sin(b);
            skyBoxColorRefracted = texture(skyBox, refracted);
        } else {
            vec3 reflected = normalize(reflect(u, vNrm));
            skyBoxColorRefracted = texture(skyBox, reflected);
        }
    }
    return skyBoxColorRefracted * material.translucency;
}

void main() {
    if(useNormalMap == 1) {
        vNrm = texture(normalMap, vTex).rgb;
        vNrm = vNrm * 2.0 - 1.0;   
        vNrm = normalize(TBN * vNrm);
        // color = vec4(1.0, 0.0, 0.0, 1.0);
    } else {
        vNrm = fNrm;
    }

    vec4 ambientColor = getAmbientColor();
    vec4 directionalColor = getDirectionalColor();
    vec4 pointLightsColor = getPointLightsColor();
    vec4 spotLightsColor = getSpotLightsColor();
    vec4 reflectedColor = getSkyBoxReflectColor();
    vec4 refractedColor = getSkyBoxRefractColor();

    // color = vec4(TBN[2], 1.0);
    color = texture(theTexture, vTex) * (ambientColor + directionalColor + pointLightsColor + spotLightsColor + reflectedColor + refractedColor);
}
