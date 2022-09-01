#version 400 core

in vec2 f_uv;

layout(location = 0) out vec4 fragment;

uniform sampler2D srcTexture;

struct HdrEffect {
    float gamma;
    float exposure;
};
uniform HdrEffect hdrEffect;

void main() {
    float gamma = hdrEffect.gamma;
    float exposure = hdrEffect.exposure;
    // HDR -> LDR
    vec3 hdrColor = texture(srcTexture, f_uv).rgb;
    // tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    // gamma correction
    vec3 color = pow(mapped, vec3(1.0 / gamma));
    fragment = vec4(color, 1.0);
}