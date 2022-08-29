#version 400 core

in vec2 f_uv;

out vec4 fragment;

uniform sampler2D screenTexture;

uniform bool enableHDR;
uniform float gamma = 2.2;
uniform float exposure = 0.1;

void main() {
    vec3 color;
    if (enableHDR) {
        // HDR -> LDR
        vec3 hdrColor = texture(screenTexture, f_uv).rgb;
        // reinhard tone mapping
        vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
        // gamma correction
        color = pow(mapped, vec3(1.0 / gamma));
    } else {
        color = texture(screenTexture, f_uv).rgb;
    }

    fragment = vec4(color, 1.0);
}