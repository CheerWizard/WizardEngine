#include ../include/diffuse.glsl
#include ../include/specular.glsl

struct Phong {
    vec4 color;

    sampler2D albedo;
    float ambient;

    sampler2D diffuseMap;
    float diffuse;

    sampler2D specularMap;
    float specular;
    float shiny;
};
const uint TEXTURE_SLOTS_PER_STAGE = 16;
const uint PHONG_MAX_COUNT = 5;

struct PhongLight {
    vec3 position;
    vec4 color;
    float ambient;
    float diffuse;
    float specular;
};

vec4 applyPhong(
    Phong material, vec2 uv, vec3 normal, vec3 viewDir,
    vec3 lightDir, PhongLight light
) {
    light.ambient *= material.ambient;
    light.diffuse *= material.diffuse;
    light.specular *= material.specular;

    vec3 diff = diff(light.color.xyz, lightDir, normal) * light.diffuse;
    vec3 spec = spec(light.color.xyz, lightDir, normal, viewDir, material.shiny) * light.specular;

    vec3 albedo = vec3(texture(material.albedo, uv));
    vec3 diffMap = vec3(texture(material.diffuseMap, uv));
    vec3 specMap = vec3(texture(material.specularMap, uv));

    vec3 amb = vec3(light.ambient);

    if (diffMap.x != 0 || diffMap.y != 0 || diffMap.z != 0) {
        amb *= diffMap;
        diff *= diffMap;
    }

    if (specMap.x != 0 || specMap.y != 0 || specMap.z != 0) {
        spec *= specMap;
    }

    vec3 result = amb * (material.color.xyz + albedo + diff + spec);

    return vec4(result, material.color.w * light.color.w);
}