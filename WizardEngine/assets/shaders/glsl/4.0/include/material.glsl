#include ambient.glsl
#include diffuse.glsl
#include specular.glsl
#include phong.glsl

struct Material {
    vec4 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shiny;
};

// normal - normalized vector of object's normal
vec3 mtl3(Material material, vec3 pos, vec3 normal, vec3 viewPos) {
    vec3 ambient = amb3() * material.ambient;
    vec3 diffuse = diff3(pos, normal) * material.diffuse;
    vec3 specular = spec3(pos, normal, viewPos, material.shiny) * material.specular;
    return ambient + diffuse + specular + material.color.xyz;
}

vec4 mtl4(Material material, vec3 pos, vec3 normal, vec3 viewPos) {
    return vec4(mtl3(material, pos, normal, viewPos), material.color.w);
}

vec3 mtl3_phong(Material material, vec3 pos, vec3 normal, vec3 viewPos) {
    return phong3(pos, normal, viewPos, material.shiny) + material.color.xyz;
}

vec4 mtl4_phong(Material material, vec3 pos, vec3 normal, vec3 viewPos) {
    return vec4(mtl3_phong(material, pos, normal, viewPos), material.color.w);
}