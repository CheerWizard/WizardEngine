#version 400 core

in vec2 f_uv;

out vec4 fragment;

uniform sampler2D screenTexture;

void main() {
    fragment = texture(screenTexture, f_uv);
}