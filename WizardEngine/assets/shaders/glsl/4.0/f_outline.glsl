#version 400

out vec4 fragment;

uniform vec4 outlineColor;

void main() {
    fragment = outlineColor;
}