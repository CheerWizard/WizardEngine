#type vertex
#version 420 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;

uniform mat4 viewProjection;
uniform mat4 transform;

out vec2 v_TextureCoords;

void main() {
    v_TextureCoords = textureCoords;
    gl_Position = viewProjection * transform * vec4(position, 1.0);
}

#type fragment
#version 420 core

layout(location = 0) out vec4 color;

in vec2 v_TextureCoords;

uniform sampler2D texture;

void main() {
    color = texture(texture, v_TextureCoords);
}