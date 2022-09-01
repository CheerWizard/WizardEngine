#include ../include/diffuse.glsl
#include ../include/specular.glsl
#include ../include/gamma.glsl
#include ../include/parallax.glsl

struct Phong {
    vec4 color;

    sampler2D albedo;
    bool enableAlbedoMap;
    float ambient;

    sampler2D diffuseMap;
    bool enableDiffuseMap;
    float diffuse;

    sampler2D specularMap;
    bool enableSpecularMap;
    float specular;
    float shiny;

    bool enableBlinn;

    float gamma;

    sampler2D normalMap;
    bool enableNormalMap;

    sampler2D depthMap;
    bool enableParallaxMap;
    float heightScale;
    float minLayers;
    float maxLayers;

    float brightness;
};

const uint TEXTURE_UNITS = 30;
const uint PHONG_MAX_COUNT = TEXTURE_UNITS / 5;

struct PhongLight {
    vec3 position;
    vec4 color;
    float ambient;
    float diffuse;
    float specular;
};

vec4 applyPhong(
    Phong material,
    vec3 pos, vec2 uv, vec3 normal,
    vec3 tangent, vec3 bitangent,
    vec3 viewPos,
    PhongLight light
) {
    light.ambient *= material.ambient;
    light.diffuse *= material.diffuse;
    light.specular *= material.specular;

    vec3 lightPos = light.position;
    mat3 TBN = transpose(mat3(tangent, bitangent, normal));
    // apply TBN matrix
    pos *= TBN;
    viewPos *= TBN;
    lightPos *= TBN;

    vec3 lightDir = normalize(lightPos - pos);
    vec3 viewDir = normalize(viewPos - pos);

    if (material.enableParallaxMap) {
        uv = parallax(
            material.depthMap, uv, viewDir,
            material.heightScale, material.minLayers, material.maxLayers
        );
        // UV bounds should be [0, 1]
        // discard pixels that are out of bounds [0, 1]
        if (uv.x > 1.0 || uv.y > 1.0 || uv.x < 0.0 || uv.y < 0.0) {
            discard;
        }
    }

    if (material.enableNormalMap) {
        // apply normal map
        normal = texture(material.normalMap, uv).rgb;
        normal = normalize(normal * 2.0 - 1.0);
        normal = normalize(TBN * normal);
    }

    vec3 albedo = vec3(0);
    vec3 amb = vec3(light.ambient);
    vec3 diff = diff(light.color.xyz, lightDir, normal) * light.diffuse;
    vec3 specular;
    if (material.enableBlinn) {
        specular = specBlinn(light.color.xyz, lightDir, normal, viewDir, material.shiny);
    } else {
        specular = spec(light.color.xyz, lightDir, normal, viewDir, material.shiny);
    }
    specular *= light.specular;

    if (material.enableAlbedoMap) {
        albedo = vec3(texture(material.albedo, uv));
    }

    if (material.enableDiffuseMap) {
        vec3 diffMap = vec3(texture(material.diffuseMap, uv));
        amb *= diffMap;
        diff *= diffMap;
    }

    if (material.enableSpecularMap) {
        vec3 specMap = vec3(texture(material.specularMap, uv));
        specular *= specMap;
    }

    vec3 color = amb * (material.color.xyz + albedo + diff + specular);
    return vec4(gamma(color.rgb, material.gamma), material.color.w);
}