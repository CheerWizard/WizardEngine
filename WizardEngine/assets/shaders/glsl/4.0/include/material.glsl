#include light.glsl

struct Material {
    vec4 color;
    float ambient;
    float diffuse;
    float specular;
    float shiny;
    bool diffuseMap;
    bool specularMap;
};

struct MaterialMaps {
    sampler2D diffuse;
    sampler2D specular;
};

vec3 mtl(
    Material material,
    PhongLight phongLight,
    vec3 pos,
    vec3 normal,
    vec3 viewDir
) {
    phongLight.ambient *= material.ambient;
    phongLight.diffuse *= material.diffuse;
    phongLight.specular *= material.specular;
    return phong(phongLight, pos, normal, viewDir, material.shiny);
}

vec3 mtl(
    Material material,
    MaterialMaps materialMaps,
    vec2 uv,
    PhongLight phongLight,
    vec3 pos,
    vec3 normal,
    vec3 viewDir
) {
    phongLight.ambient *= material.ambient;
    phongLight.diffuse *= material.diffuse;
    if (material.diffuseMap) {
        vec4 diff = texture(materialMaps.diffuse, uv);
        phongLight.ambient *= diff;
        phongLight.diffuse *= diff;
    }

    phongLight.specular *= material.specular;
    if (material.specularMap) {
        phongLight.specular *= texture(materialMaps.specular, uv);
    }

    return phong(phongLight, pos, normal, viewDir, material.shiny);
}

vec3 mtl(
    Material material,
    DirectLight directLight,
    vec3 normal,
    vec3 viewDir
) {
    directLight.ambient *= material.ambient;
    directLight.diffuse *= material.diffuse;
    directLight.specular *= material.specular;
    return dir(directLight, normal, viewDir, material.shiny);
}

vec3 mtl(
    Material material,
    MaterialMaps materialMaps,
    vec2 uv,
    DirectLight directLight,
    vec3 normal,
    vec3 viewDir
) {
    directLight.ambient *= material.ambient;
    directLight.diffuse *= material.diffuse;
    if (material.diffuseMap) {
        vec4 diff = texture(materialMaps.diffuse, uv);
        directLight.ambient *= diff;
        directLight.diffuse *= diff;
    }

    directLight.specular *= material.specular;
    if (material.specularMap) {
        directLight.specular *= texture(materialMaps.specular, uv);
    }

    return dir(directLight, normal, viewDir, material.shiny);
}

vec3 mtl(
    Material material,
    PointLight pointLight,
    vec3 pos,
    vec3 normal,
    vec3 viewDir
) {
    pointLight.ambient *= material.ambient;
    pointLight.diffuse *= material.diffuse;
    pointLight.specular *= material.specular;
    return point(pointLight, pos, normal, viewDir, material.shiny);
}

vec3 mtl(
    Material material,
    MaterialMaps materialMaps,
    vec2 uv,
    PointLight pointLight,
    vec3 pos,
    vec3 normal,
    vec3 viewDir
) {
    pointLight.ambient *= material.ambient;
    pointLight.diffuse *= material.diffuse;
    if (material.diffuseMap) {
        vec4 diff = texture(materialMaps.diffuse, uv);
        pointLight.ambient *= diff;
        pointLight.diffuse *= diff;
    }

    pointLight.specular *= material.specular;
    if (material.specularMap) {
        pointLight.specular *= texture(materialMaps.specular, uv);
    }

    return point(pointLight, pos, normal, viewDir, material.shiny);
}

vec3 mtl(
    Material material,
    FlashLight flashLight,
    vec3 pos,
    vec3 normal,
    vec3 viewDir
) {
    flashLight.ambient *= material.ambient;
    flashLight.diffuse *= material.diffuse;
    flashLight.specular *= material.specular;
    return flash(flashLight, pos, normal, viewDir, material.shiny);
}

vec3 mtl(
    Material material,
    MaterialMaps materialMaps,
    vec2 uv,
    FlashLight flashLight,
    vec3 pos,
    vec3 normal,
    vec3 viewDir
) {
    flashLight.ambient *= material.ambient;
    flashLight.diffuse *= material.diffuse;
    if (material.diffuseMap) {
        vec4 diff = texture(materialMaps.diffuse, uv);
        flashLight.ambient *= diff;
        flashLight.diffuse *= diff;
    }

    flashLight.specular *= material.specular;
    if (material.specularMap) {
        flashLight.specular *= texture(materialMaps.specular, uv);
    }

    return flash(flashLight, pos, normal, viewDir, material.shiny);
}