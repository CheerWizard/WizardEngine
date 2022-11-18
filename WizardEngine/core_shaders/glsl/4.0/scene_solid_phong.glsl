#version 400 core

#include include/f_scene.glsl
#include material/solid_phong.glsl

const uint LIGHT_COUNT = 8;
uniform PhongLight phongLight[LIGHT_COUNT];

const uint MATERIAL_COUNT = 128;
uniform SolidPhong solidPhong[MATERIAL_COUNT];

void main() {
    vec3 normal = normalize(f_normal);
    vec3 viewDir = normalize(viewPosition - f_pos);
    for (uint i = 0 ; i < LIGHT_COUNT ; i++) {
        PhongLight light = phongLight[i];
        vec3 lightDir = normalize(light.position - f_pos);
        fragment += applySolidPhong(solidPhong[getId()], normal, viewDir, lightDir, light);
    }
}