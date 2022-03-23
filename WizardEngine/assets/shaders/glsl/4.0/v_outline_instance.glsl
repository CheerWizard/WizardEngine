#version 400 core

#include v_outline_attrs.glsl
#include v_outline.glsl

void main() {
    update(gl_InstanceID);
}