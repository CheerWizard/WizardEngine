#include material.glsl

in flat int f_id;
in vec2 f_uv;
in vec3 f_objPos;
in vec3 f_normal;

out vec4 fragment;

// camera view position
uniform vec3 viewPosition;
// materials for each object in scene
uniform Material material[32];

void updateFragment() {
    vec3 normal = normalize(f_normal);
    fragment = mtl4_phong(material[f_id], f_objPos, normal, viewPosition);
}