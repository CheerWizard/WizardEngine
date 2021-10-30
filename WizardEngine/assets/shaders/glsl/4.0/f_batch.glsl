#version 400 core

in vec2 f_uv;
in flat int f_id;

out vec4 fragment;

uniform sampler2D diffuseSampler[32];

void main() {
    vec4 diffColor = texture(diffuseSampler[f_id], f_uv);
    fragment = diffColor;
}