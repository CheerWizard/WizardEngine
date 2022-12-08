#version 400 core

out vec2 f_uv;

const vec2 vertices[4] = vec2[](
    vec2(-1.0, 1.0),
    vec2(-1.0, -1.0),
    vec2(1.0, 1.0),
    vec2(1.0, -1.0)
);

void main() {
    vec2 vertex = vertices[gl_VertexID];
    gl_Position = vec4(vertex, 0, 1.0);
    f_uv = vertex / 2.0 + 0.5;
}