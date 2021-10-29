#version 400 core

in vec2 f_uv;

out vec4 fragment;

uniform sampler2D diffuseSampler;

void main() {
    vec4 diffColor = texture(diffuseSampler, f_uv);
    fragment = diffColor;
}