#version 400 core

#include v_circle_attrs.glsl
layout(location = 3) in float id;
#include v_circle.glsl

void main() {
    update(id);
}