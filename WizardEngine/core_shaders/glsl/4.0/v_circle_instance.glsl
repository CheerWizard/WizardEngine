#version 400 core

#include v_circle_attrs.glsl
#include v_circle.glsl

void main() {
    update(gl_InstanceID);
}