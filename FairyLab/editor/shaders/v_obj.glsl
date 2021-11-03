#version 400 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;
layout(location = 3) in float instanceId;

out vec2 f_uv;
out vec3 f_objPos;
out vec3 f_normal;

uniform mat4 camera;
uniform mat4 transform;

void main() {
    f_uv = uv;
    int id = int(instanceId);
    vec4 objPos = transform * vec4(position, 1.0);
    f_objPos = vec3(objPos);
    f_normal = normal;
    gl_Position = camera * objPos;
}