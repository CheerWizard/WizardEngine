#version 420 core

// order of attrs should be compatible with input model data.
layout(location = 0) in vec3 position;
 in vec2 textureCoords; // vertex!

out vec2 v_TextureCoords;

layout (std140) uniform Camera {
    mat4 camera;
};

// todo consider how to move all transforms into Camera uniform block.
uniform mat4 transform;

void main() {
    v_TextureCoords = textureCoords;
    gl_Position = camera * transform * vec4(position, 1.0);
}