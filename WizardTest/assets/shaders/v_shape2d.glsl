#version 420 core

// order of attrs should be compatible with input graphics model.
layout(location = 0) in vec3 position;
 in vec2 textureCoords; // vertex!
layout(location = 2) in vec4 color;

out vec4 v_Color;
out vec2 v_TextureCoords;

uniform mat4 camera;
uniform mat4 transform;

void main() {
    v_TextureCoords = textureCoords;
    v_Color = color;
    gl_Position = camera * transform * vec4(position, 1.0);
}