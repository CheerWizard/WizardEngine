#include ../include/diffuse.glsl
#include ../include/specular.glsl
#include ../include/gamma.glsl
#include ../include/parallax.glsl

struct Material {
    vec4 color;

    sampler2D albedoSlot;
    bool enableAlbedoMap;
    float gamma;
    float brightness;

    sampler2D normalSlot;
    bool enableNormalMap;

    sampler2D depthSlot;
    bool enableParallaxMap;
    float heightScale;
    float minLayers;
    float maxLayers;

    sampler2D metallicSlot;
    bool enableMetallicMap;
    float metallic;

    sampler2D roughnessSlot;
    bool enableRoughnessMap;
    float roughness;

    sampler2D aoSlot;
    bool enableAOMap;
    float ao;
};
const int MATERIAL_TEXTURE_UNITS = 6;

struct Light {
    vec3 position;
    vec4 color;
};

float distributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float geometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = geometrySchlickGGX(NdotV, roughness);
    float ggx1 = geometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}