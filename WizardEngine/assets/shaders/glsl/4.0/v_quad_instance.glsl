#version 400 core

#include v_quad_attrs.glsl
#include v_quad.glsl

void main() {
    update(gl_InstanceID);
}