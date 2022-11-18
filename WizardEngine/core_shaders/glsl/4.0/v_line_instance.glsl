#version 400 core

#include v_line_attrs.glsl
#include v_line.glsl

void main() {
    update(gl_InstanceID);
}