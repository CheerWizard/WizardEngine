#version 400 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;
layout(location = 3) in float instanceId;

out vec2 f_uv;
out flat int f_id;

layout (std140) uniform Camera {
    mat4 camera;
};

uniform mat4 transform[128];

void main() {
    f_uv = uv;
    int i_InstanceId = int(instanceId);
    f_id = i_InstanceId;
    gl_Position = camera * transform[i_InstanceId] * vec4(position, 1.0);
}