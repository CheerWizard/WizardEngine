#version 400 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;

out vec2 f_uv;

uniform bool chaos;
uniform bool confuse;
uniform bool shake;
uniform float time;

void main() {
    gl_Position = vec4(pos, 0.0f, 1.0f);
    if (chaos) {
        float strength = 0.3;
        vec2 pos = vec2(uv.x + sin(time) * strength, uv.y + cos(time) * strength);
        f_uv = pos;
    }
    else if (confuse) {
        f_uv = vec2(1.0 - uv.x, 1.0 - uv.y);
    }
    else {
        f_uv = uv;
    }
    if (shake) {
        float strength = 0.01;
        gl_Position.x += cos(time * 10) * strength;
        gl_Position.y += cos(time * 15) * strength;
    }
}