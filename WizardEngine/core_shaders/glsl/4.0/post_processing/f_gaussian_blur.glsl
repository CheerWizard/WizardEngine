#version 400 core

in vec2 f_uv;

layout(location = 0) out vec4 fragment;

uniform sampler2D srcTexture;

struct GaussianBlurEffect {
    bool horizontal;
};

uniform GaussianBlurEffect gaussianBlurEffect;

uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
uniform bool horizontal;

void main() {
    vec2 texelOffset = 1.0 / textureSize(srcTexture, 0); // gets size of single texel
    vec3 result = texture(srcTexture, f_uv).rgb * weight[0]; // current fragment's contribution
    if (horizontal) {
        for (int i = 1; i < 5; ++i) {
            result += texture(srcTexture, f_uv + vec2(texelOffset.x * i, 0.0)).rgb * weight[i];
            result += texture(srcTexture, f_uv - vec2(texelOffset.x * i, 0.0)).rgb * weight[i];
        }
    } else {
        for (int i = 1; i < 5; ++i) {
            result += texture(srcTexture, f_uv + vec2(0.0, texelOffset.y * i)).rgb * weight[i];
            result += texture(srcTexture, f_uv - vec2(0.0, texelOffset.y * i)).rgb * weight[i];
        }
    }
    fragment = vec4(result, 1.0);
}