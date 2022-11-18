#version 400 core

#include include/f_scene.glsl
#include material/phong.glsl

layout(location = 1) out vec4 brightColor;
layout(location = 2) out int uuid;

const uint LIGHT_COUNT = 8;
uniform PhongLight phongLight[LIGHT_COUNT];

uniform Phong phong[PHONG_MAX_COUNT];

uniform int uuids[2];

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

    float brightness = dot(fragment.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > phong[getId()].brightness) {
        brightColor = vec4(fragment.rgb, 1);
    } else {
        brightColor = vec4(0, 0, 0, 1);
    }

    uuid = uuids[getId()];
}