#include material.glsl

in flat int f_id;
in flat int f_slot;
in vec2 f_uv;
in vec3 f_pos;
in vec3 f_normal;

out vec4 fragment;

uniform vec3 viewPosition;

uniform PhongLight phongLight;
const int maxPointLight = 4;
uniform PointLight pointLight[maxPointLight];

uniform Material material;
uniform MaterialMaps materialMaps;

void updateFragment() {
    vec3 normal = normalize(f_normal);
    vec3 viewDir = normalize(viewPosition - f_pos);
    vec3 pixel = vec3(0.0);
    Material currentMaterial = material;
    pixel += currentMaterial.color.xyz;

    vec3 phong = mtl(currentMaterial, materialMaps, f_uv, phongLight, f_pos, normal, viewDir);
    pixel += phong;

    for(int i = 0; i < maxPointLight ; i++) {
        vec3 point = mtl(currentMaterial, materialMaps, f_uv, pointLight[i], f_pos, normal, viewDir);
        pixel += point;
    }

    fragment = vec4(pixel, 1.0);
}