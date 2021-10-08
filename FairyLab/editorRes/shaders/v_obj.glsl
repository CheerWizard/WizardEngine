#version 420 core

// order of attrs should be compatible with input model data.
layout(location = 0) in vec3 position;
in vec2 textureCoords; // vertex!

out vec2 v_TextureCoords;

uniform mat4 camera;
uniform mat4 transform;

void main() {
    v_TextureCoords = textureCoords;
    gl_Position = camera * transform * vec4(position, 1.0);
}