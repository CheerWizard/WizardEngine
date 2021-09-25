#version 420 core

in vec4 v_Color;
in vec2 v_TextureCoords;

out vec4 f_Color;

uniform float brightness;

uniform sampler2D diffuseSampler;

void main() {
//    f_Color = vec4(v_Color.r * brightness, v_Color.g * brightness, v_Color.b * brightness, v_Color.a);
    f_Color = texture(diffuseSampler, v_TextureCoords);
}