#version 400 core

in vec2 f_uv;
in flat int f_id;

out vec4 fragment;

uniform sampler2D char[16];
uniform vec3 color;

void main() {
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(char[f_id], f_uv).r);
    fragment = vec4(color, 1.0) * sampled;
}
