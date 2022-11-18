#version 400 core

#include v_outline_attrs.glsl
layout(location = 1) in float id;
#include v_outline.glsl

void main() {
    update(id);
}