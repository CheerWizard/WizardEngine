#version 400 core

in vec2 f_uv;

layout(location = 0) out vec4 fragment;

uniform sampler2D screenTexture;

void main() {
    vec3 color = texture(screenTexture, f_uv).rgb;
    fragment = vec4(color, 1.0);
}