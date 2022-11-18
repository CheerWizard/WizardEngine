#version 400 core

in vec2 f_uv;

layout(location = 0) out vec4 fragment;

uniform sampler2D srcTexture;

struct SharpenEffect {
    float offset;
};
uniform SharpenEffect sharpenEffect;

void main() {
    float offset = sharpenEffect.offset;
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right
    );

    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );

    vec3 samples[9];
    for (uint i = 0; i < 9; i++) {
        samples[i] = vec3(texture(srcTexture, f_uv.st + offsets[i]));
    }

    vec3 color = vec3(0.0);
    for (uint i = 0; i < 9; i++) {
        color += samples[i] * kernel[i];
    }

    fragment = vec4(color, 1.0);
}