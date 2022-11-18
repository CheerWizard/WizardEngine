#version 400 core

#include v_quad_attrs.glsl
layout(location = 2) in float id;
#include v_quad.glsl

void main() {
    update(id);
}
