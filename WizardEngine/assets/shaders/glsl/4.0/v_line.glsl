out vec4 f_color;

#include include/multiple.glsl

void update(int id) {
    vec4 linePos = transform[id] * vec4(position, 1.0);
    gl_Position = camera * linePos;
    f_color = color;
}

void update(float id) {
    update(int(id));
}
