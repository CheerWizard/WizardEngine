#version 400 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 uv;

out vec2 f_uv;
out flat int f_id;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(position, 0.0, 1.0);
    f_uv = uv;
    f_id = gl_InstanceID;
}