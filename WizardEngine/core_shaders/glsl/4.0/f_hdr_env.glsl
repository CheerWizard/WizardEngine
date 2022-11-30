#version 400 core

in vec3 f_pos;

out vec4 fragment;

uniform sampler2D hdrEnv;

const vec2 invAtan = vec2(0.1591, 0.3183);

vec2 sampleSphericalMap(vec3 v) {
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main() {
    vec2 uv = sampleSphericalMap(normalize(f_pos));
    vec3 color = texture(hdrEnv, uv).rgb;
    fragment = vec4(color, 1.0);
}