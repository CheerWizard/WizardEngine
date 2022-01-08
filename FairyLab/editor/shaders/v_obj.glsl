#version 400 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;
layout(location = 3) in float instanceId;
layout(location = 4) in float textureSlot;

out vec3 f_pos;
out vec2 f_uv;
out vec3 f_normal;

uniform mat4 camera;
uniform mat4 transform;

void main() {
    vec4 objPos = transform * vec4(position, 1.0);
    f_pos = objPos.xyz;
    f_uv = uv;
    f_normal = normal;
    gl_Position = camera * objPos;
}