#version 400 core

in vec2 f_uv;
in flat int f_id;

out vec4 fragment;

uniform sampler2D diffuseSampler[32];

uniform float strength;
uniform vec3 color;

void main() {
    vec3 ambient = strength * color;
    vec4 diffColor = texture(diffuseSampler[f_id], f_uv);
    fragment = vec4(ambient, 1.0) * diffColor;
}