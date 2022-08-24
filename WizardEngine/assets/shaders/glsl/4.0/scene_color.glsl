#version 400 core

#include include/f_scene.glsl

const uint COLOR_COUNT = 128;
uniform vec4 color[COLOR_COUNT];

void main() {
    fragment = color[getId()];
}