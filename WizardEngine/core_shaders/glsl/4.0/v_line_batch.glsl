#version 400 core

#include v_line_attrs.glsl
layout(location = 2) in float id;
#include v_line.glsl

void main() {
    update(id);
}