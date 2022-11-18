#version 400 core

in vec2 f_uv;
out vec4 fragment;

uniform sampler2D sceneSampler;
uniform vec2      offsets[9];
uniform int       edge_kernel[9];
uniform float     blur_kernel[9];

uniform bool chaos;
uniform bool confuse;
uniform bool shake;

void main() {
    color = vec4(0.0f);
    vec3 samples[9];
    // sample from texture offsets if using convolution matrix
    if (chaos || shake) {
        for(int i = 0; i < 9; i++) {
            samples[i] = vec3(texture(sceneSampler, f_uv.st + offsets[i]));
        }
    }
    // process effects
    if (chaos) {
        for(int i = 0; i < 9; i++) {
            fragment += vec4(samples[i] * edge_kernel[i], 0.0f);
            fragment.a = 1.0f;
        }
    }
    else if (confuse) {
        fragment = vec4(1.0 - texture(sceneSampler, f_uv).rgb, 1.0);
    }
    else if (shake) {
        for(int i = 0; i < 9; i++) {
            fragment += vec4(samples[i] * blur_kernel[i], 0.0f);
            fragment.a = 1.0f;
        }
    }
    else {
        fragment = texture(sceneSampler, f_uv);
    }
}