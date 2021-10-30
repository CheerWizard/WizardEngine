#version 420 core

in vec2 f_uv;

out vec4 fragment;

uniform sampler2D diffuseSampler[1];
uniform vec4 color;

void main() {
    fragment = texture(diffuseSampler[0], f_uv);
}