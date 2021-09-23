#version 420 core

layout(location = 0) in vec3 position;
//layout(location = 1) in vec4 color;

//out vec4 v_Color;

//uniform mat4 viewProjection;
//uniform mat4 transform;

void main() {
    gl_Position = vec4(position, 1.0);
//    v_Color = color;
}