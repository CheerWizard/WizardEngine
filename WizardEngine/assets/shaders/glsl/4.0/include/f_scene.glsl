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

//void updateFragment() {
//    vec3 normal = normalize(f_normal);
//    vec3 viewDir = normalize(viewPosition - f_pos);
//    fragment = vec4(0.5, 0.5, 0.5, 1.0);
//}