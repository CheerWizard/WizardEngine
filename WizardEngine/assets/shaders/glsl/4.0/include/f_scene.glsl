in flat int f_id;
in vec3 f_pos;
in vec2 f_uv;
in vec3 f_normal;

out vec4 fragment;

uniform vec3 viewPosition;

void updateFragment() {
    vec3 normal = normalize(f_normal);
    vec3 viewDir = normalize(viewPosition - f_pos);
    fragment = vec4(0.3, 0, 0.7, 1.0);
}