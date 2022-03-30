#version 400 core

in vec2 f_uv;
in flat int f_id;

out vec4 fragment;

uniform sampler2D bitmap;
uniform vec4 color[128];

void main() {
    vec4 char = vec4(color[f_id].rgb, texture(bitmap, f_uv).r);
    if (char.a == 0) {
        discard;
    }
    fragment = char;
}
