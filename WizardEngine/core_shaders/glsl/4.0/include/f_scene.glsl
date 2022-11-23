in flat int f_id;
in vec3 f_pos;
in vec2 f_uv;
in vec3 f_normal;
in vec3 f_tangent;
in vec3 f_bitangent;

layout(location = 0) out vec4 fragment;

uniform vec3 viewPosition;

uniform int entityId[128];

uint getId() {
    return f_id;
}