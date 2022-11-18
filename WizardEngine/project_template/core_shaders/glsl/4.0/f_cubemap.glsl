#version 400 core

in vec3 f_dir;
uniform samplerCube cubeMap;

out vec4 fragment;

void main() {
    fragment = texture(cubeMap, f_dir);
}
