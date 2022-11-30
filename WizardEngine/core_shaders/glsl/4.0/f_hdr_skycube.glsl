#version 400 core

in vec3 f_pos;

out vec4 fragment;

uniform samplerCube cubemap;

void main() {
    vec3 envColor = texture(cubemap, f_pos).rgb;
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2));
    fragment = vec4(envColor, 1.0);
}