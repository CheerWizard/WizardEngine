#version 400 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in float instanceId;

out vec2 f_uv;

uniform mat4 camera;
uniform mat4 transform[100];

void main() {
    f_uv = uv;
    int _instanceId = int(instanceId);
    gl_Position = camera * transform[_instanceId] * vec4(position, 1.0);
}