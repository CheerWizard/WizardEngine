#version 400 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in float id;

out vec2 f_uv;
out flat int f_id;

uniform mat4 camera;
uniform mat4 transform[128];

void main() {
    int int_id = int(id);
    gl_Position = camera * transform[int_id] * vec4(position, 0.0, 1.0);
    f_uv = uv;
    f_id = int_id;
}