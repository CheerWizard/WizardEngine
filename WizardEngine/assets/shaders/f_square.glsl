#version 420 core

in vec2 v_uv;

out vec4 f_Color;

uniform sampler2D diffuseSampler;

void main() {
    f_Color = texture(diffuseSampler, v_uv);
}