#version 400 core

out vec4 fragment;

in vec2 f_uv;

const uint TEXTURE_UNITS = 16;
uniform sampler2D textures[TEXTURE_UNITS];
uniform bool enabled[TEXTURE_UNITS];

void main() {
    vec3 mix = texture(textures[0], f_uv).rgb;
    for (uint i = 1; i < TEXTURE_UNITS; i++) {
        if (enabled[i]) {
            mix += texture(textures[i], f_uv).rgb;
        }
    }
    fragment = vec4(mix, 1.0);
}