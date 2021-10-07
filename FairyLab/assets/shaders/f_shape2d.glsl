#version 420 core

in vec2 v_TextureCoords;

out vec4 f_Color;

uniform sampler2D diffuseSampler;

void main() {
    vec4 diffColor = texture(diffuseSampler, v_TextureCoords);
    f_Color = vec4(0.8, 0.8, 0.8, 1.0);
}