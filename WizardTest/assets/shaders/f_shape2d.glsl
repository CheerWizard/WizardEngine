#version 420 core

in vec4 v_Color;
in vec2 v_TextureCoords;

out vec4 f_Color;

uniform sampler2D diffuseSampler;

void main() {
    vec4 diffColor = texture(diffuseSampler, v_TextureCoords);
    f_Color = diffColor;
}