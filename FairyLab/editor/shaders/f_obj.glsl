#version 400 core
#include include/material.glsl

in vec3 f_pos;
in vec2 f_uv;
in vec3 f_normal;

out vec4 fragment;

uniform vec3 viewPosition;
uniform PhongLight phongLight;
uniform Material material;

void main() {
    vec3 normal = normalize(f_normal);
    vec3 viewDir = normalize(viewPosition - f_pos);
    vec3 pixel = vec3(0.0);
    Material currentMaterial = material;
    vec3 phong = mtl(currentMaterial, phongLight, f_pos, normal, viewDir);
    pixel += currentMaterial.color.xyz;
    pixel += phong;
    fragment = vec4(pixel, 1.0);
}