#version 400 core

layout (location = 0) in vec3 position;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;

out vec3 f_pos;

void main() {
    f_pos = position;
    mat4 rotView = mat4(mat3(view)); // remove translation from the view matrix
    vec4 clipPos = projection * rotView * vec4(position, 1.0);
    gl_Position = clipPos.xyww;
}