#version 400 core

#include include/f_scene.glsl
#include material/phong.glsl

const uint LIGHT_COUNT = 8;
uniform PhongLight phongLight[LIGHT_COUNT];

uniform Phong phong[PHONG_MAX_COUNT];

void main() {
    for (uint i = 0 ; i < LIGHT_COUNT ; i++) {
        PhongLight light = phongLight[i];
        vec3 lightDir = normalize(light.position - f_pos);
        fragment += applyPhong(
            phong[getId()],
            f_pos, f_uv, f_normal,
            f_tangent, f_bitangent,
            viewPosition,
            light
        );
    }
}