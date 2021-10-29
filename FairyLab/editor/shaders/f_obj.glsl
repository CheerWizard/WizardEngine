#version 420 core

in vec2 f_uv;

out vec4 fragment;

uniform vec4 color;

void main() {
    fragment = color;
}