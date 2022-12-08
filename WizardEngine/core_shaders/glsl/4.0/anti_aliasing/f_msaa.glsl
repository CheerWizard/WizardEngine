#version 400 core

out vec4 fragment;

in vec2 f_uv;

uniform sampler2DMS msaaSampler;

uniform int vp_width;
uniform int vp_height;
uniform int samples;

void main() {
    ivec2 uv = ivec2(vp_width, vp_height);
    uv.x = int(uv.x * f_uv.x);
    uv.y = int(uv.y * f_uv.y);

    vec4 s;
    for (int i = 0; i < samples; i++) {
        s += texelFetch(msaaSampler, uv, i);
    }

    fragment = s / 4.0f;
}