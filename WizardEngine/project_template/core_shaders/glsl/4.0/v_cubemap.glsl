#version 400 core

layout (location = 0) in vec3 position;

out vec3 f_dir;

#include include/single.glsl

void main() {
    f_dir = position;
    vec4 pos = camera * transform * vec4(position, 1.0);
    gl_Position = pos.xyww;
}