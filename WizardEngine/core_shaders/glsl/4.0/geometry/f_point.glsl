#version 400 core

in vec3 f_color;
out vec4 fragment;

void main() {
    fragment = vec4(f_color, 1.0);
}