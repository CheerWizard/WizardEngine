#version 420 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;
layout(location = 2) in vec4 color;

out vec4 v_Color;
out vec2 v_TextureCoords;

uniform mat4 projection;
uniform mat4 transform;

void main() {
    v_TextureCoords = textureCoords;
    v_Color = color;
    gl_Position = projection * vec4(position, 1.0);
}