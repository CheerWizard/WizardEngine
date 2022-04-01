#version 400 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 uv;

out vec2 f_uv;

void main() {
    f_uv = uv;
    gl_Position = vec4(position.xy, 1, 1);
}