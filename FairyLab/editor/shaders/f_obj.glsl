#version 420 core

in vec2 f_uv;

out vec4 fragment;

uniform sampler2D diffuseSampler[1];

uniform float strength;
uniform vec3 color;

void main() {
    vec3 ambient = strength * color;
    vec4 objColor = vec4(ambient, 1.0) * texture(diffuseSampler[0], f_uv);
    fragment = objColor;
}