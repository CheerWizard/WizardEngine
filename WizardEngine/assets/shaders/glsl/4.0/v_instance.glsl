#version 400 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in float instanceId;

out vec2 f_uv;

layout (std140) uniform Camera {
    mat4 camera;
};

uniform mat4 transform[128];

void main() {
    f_uv = uv;
    int id = int(instanceId);
    gl_Position = camera * transform[gl_InstanceID] * vec4(position, 1.0);
}
