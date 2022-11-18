#include ../include/diffuse.glsl
#include ../include/specular.glsl

struct SolidPhong {
    vec4 color;
    float ambient;
    float diffuse;
    float specular;
    float shiny;
};

struct PhongLight {
    vec3 position;
    vec4 color;
    float ambient;
    float diffuse;
    float specular;
};

vec4 applySolidPhong(
    SolidPhong material, vec3 normal, vec3 viewDir,
    vec3 lightDir, PhongLight light
) {
    light.ambient *= material.ambient;
    light.diffuse *= material.diffuse;
    light.specular *= material.specular;

    vec3 diff = diff(light.color.xyz, lightDir, normal) * light.diffuse;
    vec3 spec = spec(light.color.xyz, lightDir, normal, viewDir, material.shiny) * light.specular;

    vec3 result = material.color.xyz * light.ambient * (diff + spec);

    return vec4(result, material.color.w * light.color.w);
}