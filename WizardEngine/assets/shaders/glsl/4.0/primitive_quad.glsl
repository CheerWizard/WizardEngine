#version 400 core

out vec2 f_uv;

const vec2 vertices[4] = vec2[](
    vec2(-1.0, 1.0),
    vec2(-1.0, -1.0),
    vec2(1.0, 1.0),
    vec2(1.0, -1.0)
);

void main() {
    gl_Position = vec4(vertices[gl_VertexID], 0, 1.0);
    f_uv = (gl_Position.xy) / 2.0 + 0.5;
}