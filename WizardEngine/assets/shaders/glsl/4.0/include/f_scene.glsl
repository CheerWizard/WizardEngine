in flat int f_id;
in vec3 f_pos;
in vec2 f_uv;
in vec3 f_normal;

layout(location = 0) out vec4 fragment;
layout(location = 1) out int outEntityId;

uniform vec3 viewPosition;

uniform int entityId[128];

void updateFragment() {
    vec3 normal = normalize(f_normal);
    vec3 viewDir = normalize(viewPosition - f_pos);
    outEntityId = entityId[f_id];
    fragment = vec4(0.5, 0.5, 0.5, 1.0);
}