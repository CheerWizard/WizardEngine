#version 400 core
#include include/v_scene.glsl

out vec2 f_uv;
out flat int f_id;
out vec3 f_objPos;
out vec3 f_normal;

layout (std140) uniform Camera {
    mat4 camera;
};

uniform mat4 transform[128];

void main() {
    f_uv = uv;
    int i_InstanceId = int(instanceId);
    f_id = i_InstanceId;
    vec4 objPos = transform[i_InstanceId] * vec4(position, 1.0);
    f_objPos = vec3(objPos);
    f_normal = normal;
    gl_Position = camera * objPos;
}