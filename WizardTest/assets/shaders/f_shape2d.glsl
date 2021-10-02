#version 420 core

in vec4 v_Color;
in vec2 v_TextureCoords;

out vec4 f_Color;

uniform sampler2D diffuseSampler;
uniform sampler2D diffuseSampler2;

void main() {
    sampler2D diffSampler = diffuseSampler;
    vec4 diffColor = texture(diffSampler, v_TextureCoords);
    f_Color = diffColor;
}